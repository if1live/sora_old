/*  Copyright (C) 2011 by if1live */
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
#include "yukino_stdafx.h"
#include "renderer.h"

#include "sora/matrix.h"
#include "sora/math_helper.h"
#include "sora/shader.h"

namespace yukino {;
const char vert_src[] = " \
  uniform mat4 u_projection;  \
  uniform mat4 u_modelview; \
attribute vec3 a_position;  \
attribute vec2 a_texcoord;    \
varying vec2 v_texcoord;  \
void main() { \
	v_texcoord = a_texcoord;  \
  gl_Position = u_projection * u_modelview * vec4(a_position, 1.0);   \
}";

const char frag_src[] = " \
precision mediump float;  \
varying lowp vec2 v_texcoord; \
uniform sampler2D s_texture;  \
void main() { \
gl_FragColor = texture2D(s_texture, v_texcoord);  \
}";

struct Vertex {
  float xyz[3];
  float st[2];
};

class RendererPimpl {
public:
  RendererPimpl()
    : mode(0), curr_index(0), matrix_mode(kProjection) {
    // initialize program
    prog.Init(vert_src, frag_src);

    // make quad index
    int base_index = 0;
    for (int i = 0 ; i < kTriangleCount / 2; i++) {
      SR_ASSERT(base_index+5 < kTriangleCount * 3);
      index_list[base_index+0] = i*3 + 0;
      index_list[base_index+1] = i*3 + 1;
      index_list[base_index+2] = i*3 + 2;

      index_list[base_index+3] = i*3 + 0;
      index_list[base_index+4] = i*3 + 2;
      index_list[base_index+5] = i*3 + 3;

      base_index += 6;
    }
    // matrix stack를 적절히 초기화
    sora::SetIdentity(&projection_mat);
    sora::SetIdentity(&modelview_mat);
  }

  sora::Program prog;
  static const int kTriangleCount = 2048;
  static const int kVertexCount = kTriangleCount * 3;
  Vertex vertex_list[kVertexCount];
  // GL_QUAD 구현에 사용됨
  GLushort index_list[kTriangleCount * 3];
  Vertex vert;
  int curr_index;
  GLenum mode;

  //matrix stack
  typedef std::vector<sora::mat4> MatrixStackType;
  enum {
    kProjection,
    kModelview,
  };
  int matrix_mode;
  sora::mat4 &GetCurrMatrix() {
    if (matrix_mode == kProjection) {
      return projection_mat;
    } else {
      return modelview_mat;
    }
  }
  MatrixStackType matrix_stack;
  sora::mat4 projection_mat;
  sora::mat4 modelview_mat;

  // shader location
  int modelview_location;
  int projection_location;
};

Renderer::Renderer()
: impl_(NULL) {
}
Renderer::~Renderer() {
  if (impl_ != NULL) {
    delete(impl_);
    impl_ = NULL;
  }
}
void Renderer::Init() {
  impl_ = new RendererPimpl();
  
  using sora::Program;
  Program &prog = impl_->prog;
  srglUseProgram(prog.prog);
  GLint modelview_location = prog.GetUniformLocation("u_modelview");
  GLint projection_location = prog.GetUniformLocation("u_projection");
  GLint texcoord_location = prog.GetAttribLocation("a_texcoord");
  GLint position_location = prog.GetAttribLocation("a_position");
  SR_ASSERT(modelview_location != -1);
  SR_ASSERT(projection_location != -1);
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(position_location != -1);
  impl_->projection_location = projection_location;
  impl_->modelview_location =modelview_location;
  
  srglEnableVertexAttribArray(texcoord_location);
  srglEnableVertexAttribArray(position_location);
  
  const void *position_ptr = impl_->vertex_list[0].xyz;
  const void *texcoord_ptr = impl_->vertex_list[0].st;
  srglVertexAttribPointer(position_location, 3, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), position_ptr);
  srglVertexAttribPointer(texcoord_location, 2, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), texcoord_ptr);

  // default set
  sora::mat4 identity;
  sora::SetIdentity(&identity);
  srglUniformMatrix4fv(projection_location, 1, GL_FALSE, identity.value);
  srglUniformMatrix4fv(modelview_location, 1, GL_FALSE, identity.value);

  // GL환경 설정
  srglEnable(GL_TEXTURE_2D);
  srglEnable(GL_BLEND);
  srglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Renderer::Begin(GLenum mode) {
  SR_ASSERT(Renderer::GetInstance().impl_ != NULL);

  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  renderer->mode = mode;
  renderer->curr_index = 0;
  memset(&renderer->vert, 0, sizeof(Vertex));
}
void Renderer::Vertex3f(float x, float y, float z) {
  SR_ASSERT(Renderer::GetInstance().impl_ != NULL);

  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  renderer->vert.xyz[0] = x;
  renderer->vert.xyz[1] = y;
  renderer->vert.xyz[2] = z;

  renderer->vertex_list[renderer->curr_index] = renderer->vert;
  renderer->curr_index++;
}
void Renderer::TexCoord2f(float s, float t) {
  SR_ASSERT(Renderer::GetInstance().impl_ != NULL);

  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  renderer->vert.st[0] = s;
  renderer->vert.st[1] = t;
}
void Renderer::End() {
  SR_ASSERT(Renderer::GetInstance().impl_ != NULL);

  // modelview, projection을 적절히 적용
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  using sora::mat4;
  mat4 &projection = renderer->projection_mat;
  mat4 &modelview = renderer->modelview_mat;
  srglUniformMatrix4fv(renderer->projection_location, 1, GL_FALSE, projection.value);
  srglUniformMatrix4fv(renderer->modelview_location, 1, GL_FALSE, modelview.value);

  // draw mesh
  if (renderer->mode == GL_QUADS) {
    srglDrawElements(GL_TRIANGLES, 
      renderer->curr_index * 3 / 2,
      GL_UNSIGNED_SHORT, 
      renderer->index_list);
  } else {
    srglDrawArrays(renderer->mode, 0, renderer->curr_index);
  }
}

void Renderer::UseProjectionMatrixMode() {
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  renderer->matrix_mode = RendererPimpl::kProjection;
}
void Renderer::UseModelviewMatrixMode() {
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  renderer->matrix_mode = RendererPimpl::kModelview;
}
void Renderer::SetMatrixToIdentity() {
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  sora::mat4 &m = renderer->GetCurrMatrix();
  sora::SetIdentity(&m);
}
void Renderer::PushMatrix() {
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  sora::mat4 &m = renderer->GetCurrMatrix();
  renderer->matrix_stack.push_back(m);
}
void Renderer::PopMatrix() {
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  SR_ASSERT(renderer->matrix_stack.empty() == false);
  sora::mat4 &back = renderer->matrix_stack.back();
  renderer->matrix_stack.pop_back();
  sora::mat4 &m = renderer->GetCurrMatrix();
  m = back;
}
void Renderer::Scale(float x, float y, float z) {
  sora::mat4 scale;
  sora::SetScale(x, y, z, &scale);
  MultMatrix(scale.value);
}
void Renderer::Translate(float x, float y, float z) {
  sora::mat4 translate;
  sora::SetTranslate(x, y, z, &translate);
  MultMatrix(translate.value);
}
void Renderer::Rotate(float degree, float x, float y, float z) {
  sora::mat4 rotate;
  if (x == 0 && y == 0 && z == 1) {
    sora::SetRotateZ(degree, &rotate);
  } else if (x == 0 && y == 1 && z == 0) {
    sora::SetRotateY(degree, &rotate);
  } else if (x == 1 && y == 0 && z == 0) {
    sora::SetRotateX(degree, &rotate);
  } else {
    SR_ASSERT(!"not support rotation");
  }
  MultMatrix(rotate.value);
}
void Renderer::MultMatrix(float *m) {
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  sora::mat4 &curr_mat = renderer->GetCurrMatrix();
  sora::mat4 tmp(m);
  curr_mat *= tmp;
}
void Renderer::MatrixMode(int matrix_mode) {
  switch (matrix_mode) {
  case SR_PROJECTION:
#ifdef GL_PROJECTION
  case GL_PROJECTION:
#endif
    UseProjectionMatrixMode();
    break;
  case SR_MODELVIEW:
#ifdef GL_MODELVIEW
  case GL_MODELVIEW:
#endif
    UseModelviewMatrixMode();
    break;
  }
}

void Renderer::LookAt(float eye_x, float eye_y, float eye_z,
  float target_x, float target_y, float target_z,
  float up_x, float up_y, float up_z) {
  sora::mat4 lookat;
  sora::SetLookAt(eye_x, eye_y, eye_z,
    target_x, target_y, target_z,
    up_x, up_y, up_z, &lookat);
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  sora::mat4 &curr_mat = renderer->GetCurrMatrix();
  curr_mat *= lookat;
}
void Renderer::Perspective(float fovy, float aspect, float zNear, float zFar) {
  sora::mat4 tmp;
  sora::SetPerspective(fovy, aspect, zNear, zFar, &tmp);
  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  sora::mat4 &curr_mat = renderer->GetCurrMatrix();
  curr_mat *= tmp;
}

}