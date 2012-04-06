/*  Copyright (C) 2011-2012 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "sora_stdafx.h"
#include "selection_buffer.h"
#include "gl_helper.h"
#include "shader.h"
#include "sys/memory_file.h"
#include "sys/filesystem.h"

using namespace sora;

namespace sora {;

SelectionBuffer::SelectionBuffer() {
  memset(this, 0, sizeof(SelectionBuffer));
}
SelectionBuffer::~SelectionBuffer() {
  if(selection_shader_ != NULL) {
    delete(selection_shader_);
    selection_shader_ = NULL;
  }
}

void SelectionBuffer::Init(int w, int h) {
  //create selection buf
  width_ = w;
  height_ = h;

  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  //depth
  glGenRenderbuffers(1, &depth_);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
  GLHelper::CheckError("Create Depth Buffer");
  //color
  //텍스쳐가 아니라 버퍼로 생성할 경우 타입에 제약이 걸린다
  //http://www.khronos.org/opengles/sdk/docs/man/xhtml/glRenderbufferStorage.xml
  //에 따르면 색깔버퍼용 타입은 사실상
  //GL_RGBA4, GL_RGB565, GL_RGB5_A1 뿐이다. 뭐 어차피 선택버퍼는
  //컬러버퍼는 작아도 상관없으니까 대충 하나 쓰자
  //라고 할라고 햇는데 문제는 int형으로 저장햇기 떄문에 RGBA8이 아니면 데이터가 날라간다
  //그래서 render to texture를 쓰도록할것!
  /*
  glGenRenderbuffers(1, &color_);
  glBindRenderbuffer(GL_RENDERBUFFER, color_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, w, h);
  */
  glGenTextures(1, &color_);
  glBindTexture(GL_TEXTURE_2D, color_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  GLHelper::CheckError("Create Color Buffer");

  //attach to fb
  //renderbuffer생성하자마자 달면 GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 에러 뜬다
  //이놈은 reference에도 바로 안보이는 더러운 놈이다
  //http://www.opengl.org/wiki/Framebuffer_Object
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_);
  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_, 0);
  GLHelper::CheckFrameBufferStatus("fb");
  GLHelper::CheckError("Create Selection Buffer");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //쉐이더 적절히 생성
  selection_shader_ = new ShaderProgram();
  std::string app_vert_path = sora::Filesystem::GetAppPath("shader/selection.vs");
  std::string app_frag_path = sora::Filesystem::GetAppPath("shader/selection.fs");
  sora::MemoryFile vert_file(app_vert_path);
  sora::MemoryFile frag_file(app_frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  bool prog_result = selection_shader_->Init(vert_src, frag_src);
  if(prog_result == false) {
    LOGE("Could not create program.");
  }
}
void SelectionBuffer::Deinit() {
  if(fbo_ != 0) {
    glDeleteFramebuffers(1, &fbo_);
    fbo_ = 0;
  }
  if(color_ != 0) {
    glDeleteTextures(1, &color_);
    //glDeleteRenderbuffers(1, &color_);
    color_ = 0;
  }
  if(depth_ != 0) {
    glDeleteRenderbuffers(1, &depth_);
    depth_ = 0;
  }
}

void SelectionBuffer::BeginDraw(int gl_x, int gl_y) {
  if(fbo_ == 0) {
    return;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //터치한곳 근처만 그려서 부하를 줄이자
  //이 대가로 새로 찍을떄마다 새로 그려야할것이다.
  //하지만 매 프레임마도 특정지점 요청을 지속적으로 할 일은
  //아마도 없을테니 당분간은 큰 문제가 없을듯
  //scissor test로 일부만 그리기
  glEnable(GL_SCISSOR_TEST);
  glScissor(gl_x-1, gl_y-1, 2, 2);
}

int SelectionBuffer::EndDraw(int gl_x, int gl_y) {
  if(fbo_ == 0) {
    return -1;
  }
  glDisable(GL_SCISSOR_TEST);
  unsigned char pixel_data[4];
  glReadPixels(gl_x, gl_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
  int color_id = (*(int*)pixel_data);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return color_id;
}

void SelectionBuffer::IdToArray(int color_id, int arr[4]) {
  //color_id를 ivec4로 변환하기 위해서 char배열로 변환후 다시 조합하자
  char tmp_color_id[4];
  memcpy(tmp_color_id, &color_id, sizeof(tmp_color_id));
  for(int i = 0 ; i < 4 ; i++) {
    arr[i] = tmp_color_id[i];
  }
}

int SelectionBuffer::GetColorIdLocation() const {
  if(selection_shader_ == NULL) {
    return -1;
  }
  return selection_shader_->GetUniformLocation("u_colorId");
}
//////////////////////////////////

SelectionRequest::SelectionRequest(SelectionBuffer *buf, int gl_x, int gl_y)
: buffer_(buf), gl_x_(gl_x), gl_y_(gl_y) {
  buffer_->BeginDraw(gl_x, gl_y);
}
SelectionRequest::~SelectionRequest() {
  if(buffer_ != NULL) {
    buffer_->EndDraw(gl_x_, gl_y_);
  }
}
int SelectionRequest::GetId() {
  int color_id = buffer_->EndDraw(gl_x_, gl_y_);
  buffer_ = NULL;
  return color_id;
}


}