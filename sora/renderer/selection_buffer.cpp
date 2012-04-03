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

using namespace sora;

namespace sora {;

SelectionBuffer::SelectionBuffer() {
  memset(this, 0, sizeof(SelectionBuffer));
}
SelectionBuffer::~SelectionBuffer() {
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
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_);
  GLHelper::CheckFrameBufferStatus("fb");

  //color
  glGenRenderbuffers(1, &color_);
  glBindRenderbuffer(GL_RENDERBUFFER, color_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_);
  GLHelper::CheckFrameBufferStatus("fb");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void SelectionBuffer::Deinit() {
  if(fbo_ != 0) {
    glDeleteFramebuffers(1, &fbo_);
    fbo_ = 0;
  }
  if(color_ != 0) {
    glDeleteRenderbuffers(1, &color_);
    color_ = 0;
  }
  if(depth_ != 0) {
    glDeleteRenderbuffers(1, &depth_);
    depth_ = 0;
  }
}

void SelectionBuffer::BeginDraw(int gl_x, int gl_y) {
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