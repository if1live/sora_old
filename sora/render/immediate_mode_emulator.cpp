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
#include "sora_stdafx.h"
#include "immediate_mode_emulator.h"

#include "sora/matrix.h"
#include "sora/math_helper.h"
#include "gl_inc.h"
#include "matrix_stack.h"
#include "basic_shader.h"

#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace sora {;

struct Vertex {
  float xyz[3];
  float st[2];
  float rgba[4];
};

class ImmediateModeEmulatorImpl {
public:
  ImmediateModeEmulatorImpl()
    : mode(0), curr_index(0) {
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

    for (int i = 0 ; i < 4 ; i++) {
      color[i] = 1.0f;
    }
  }

  static const int kTriangleCount = 2048;
  static const int kVertexCount = kTriangleCount * 3;
  Vertex vertex_list[kVertexCount];
  // GL_QUAD 구현에 사용됨
  GLushort index_list[kTriangleCount * 3];
  Vertex vert;
  int curr_index;
  GLenum mode;

  //color값은 1회 설정한것이 지속된다
  float color[4];
};

ImmediateModeEmulator::ImmediateModeEmulator()
: impl_(NULL) {
}
ImmediateModeEmulator::~ImmediateModeEmulator() {
  if (impl_ != NULL) {
    delete(impl_);
    impl_ = NULL;
  }
}
void ImmediateModeEmulator::Init() {
  impl_ = new ImmediateModeEmulatorImpl();
  
  BasicShader &basic_shader = BasicShader::GetInstance();
  basic_shader.Use();
  
  srglEnableVertexAttribArray(basic_shader.texcoord_location);
  srglEnableVertexAttribArray(basic_shader.position_location);
  srglEnableVertexAttribArray(basic_shader.color_location);
  
  const void *position_ptr = impl_->vertex_list[0].xyz;
  const void *texcoord_ptr = impl_->vertex_list[0].st;
  const void *color_ptr = impl_->vertex_list[0].rgba;
  srglVertexAttribPointer(basic_shader.position_location, 3, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), position_ptr);
  srglVertexAttribPointer(basic_shader.texcoord_location, 2, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), texcoord_ptr);
  srglVertexAttribPointer(basic_shader.color_location, 4, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), color_ptr);

  // default set
  mat4 identity;
  SetIdentity(&identity);
  srglUniformMatrix4fv(basic_shader.projection_location, 1, GL_FALSE, identity.value);
  srglUniformMatrix4fv(basic_shader.modelview_location, 1, GL_FALSE, identity.value);

  // GL환경 설정
  srglEnable(GL_TEXTURE_2D);
  srglEnable(GL_BLEND);
  srglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

ImmediateModeEmulatorImpl *ImmediateModeEmulator::impl() {
  ImmediateModeEmulatorImpl *impl = ImmediateModeEmulator::GetInstance().impl_;
  SR_ASSERT(impl != NULL);
  return impl;
}

void ImmediateModeEmulator::Begin(GLenum mode) {
  ImmediateModeEmulatorImpl *renderer = impl();
  renderer->mode = mode;
  renderer->curr_index = 0;

  memset(&renderer->vert, 0, sizeof(Vertex));
}
void ImmediateModeEmulator::Vertex3f(float x, float y, float z) {
  ImmediateModeEmulatorImpl *renderer = impl();
  renderer->vert.xyz[0] = x;
  renderer->vert.xyz[1] = y;
  renderer->vert.xyz[2] = z;

  memcpy(renderer->vert.rgba, renderer->color, 4 * sizeof(float));

  renderer->vertex_list[renderer->curr_index] = renderer->vert;
  renderer->curr_index++;
}
void ImmediateModeEmulator::TexCoord2f(float s, float t) {
  ImmediateModeEmulatorImpl *renderer = impl();
  renderer->vert.st[0] = s;
  renderer->vert.st[1] = t;
}
void ImmediateModeEmulator::End() {
  ImmediateModeEmulatorImpl *renderer = impl();

  // modelview, projection을 적절히 적용
  MatrixStack &mat_stack = MatrixStack::GetInstance();
  const mat4 &projection = mat_stack.projection_mat();
  const mat4 &modelview = mat_stack.modelview_mat();

  BasicShader &basic_shader = BasicShader::GetInstance();
  srglUniformMatrix4fv(basic_shader.projection_location, 1, GL_FALSE, projection.value);
  srglUniformMatrix4fv(basic_shader.modelview_location, 1, GL_FALSE, modelview.value);

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

void ImmediateModeEmulator::Color4f(float r, float g, float b, float a) {
  ImmediateModeEmulatorImpl *renderer = impl();
  renderer->color[0] = r;
  renderer->color[1] = g;
  renderer->color[2] = b;
  renderer->color[3] = a;
}
}