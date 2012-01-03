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

#include "matrix.h"
#include "math_helper.h"
#include "shader.h"
#include "gl_inc.h"
#include "matrix_stack.h"

namespace sora {;
const char vert_src[] = " \
  uniform mat4 u_projection;  \
  uniform mat4 u_modelview; \
attribute vec3 a_position;  \
attribute vec2 a_texcoord;    \
attribute vec4 a_color; \
varying vec2 v_texcoord;  \
varying vec4 v_color; \
void main() { \
	v_texcoord = a_texcoord;  \
  v_color = a_color;  \
  gl_Position = u_projection * u_modelview * vec4(a_position, 1.0);   \
}";

const char frag_src[] = " \
precision mediump float;  \
varying lowp vec2 v_texcoord; \
varying lowp vec4 v_color; \
uniform sampler2D s_texture;  \
void main() { \
gl_FragColor = v_color * texture2D(s_texture, v_texcoord);  \
}";

struct Vertex {
  float xyz[3];
  float st[2];
  float rgba[4];
};

class ImmediateModeEmulatorImpl {
public:
  ImmediateModeEmulatorImpl()
    : mode(0), curr_index(0) {
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

    for (int i = 0 ; i < 4 ; i++) {
      color[i] = 1.0f;
    }
  }

  Program prog;
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

  // shader location
  int modelview_location;
  int projection_location;
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
  
  Program &prog = impl_->prog;
  srglUseProgram(prog.prog);
  GLint modelview_location = prog.GetUniformLocation("u_modelview");
  GLint projection_location = prog.GetUniformLocation("u_projection");
  SR_ASSERT(modelview_location != -1);
  SR_ASSERT(projection_location != -1);
  impl_->projection_location = projection_location;
  impl_->modelview_location =modelview_location;
  
  GLint texcoord_location = prog.GetAttribLocation("a_texcoord");
  GLint position_location = prog.GetAttribLocation("a_position");
  GLint color_location = prog.GetAttribLocation("a_color");
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(position_location != -1);
  SR_ASSERT(color_location != -1);
  srglEnableVertexAttribArray(texcoord_location);
  srglEnableVertexAttribArray(position_location);
  srglEnableVertexAttribArray(color_location);
  
  const void *position_ptr = impl_->vertex_list[0].xyz;
  const void *texcoord_ptr = impl_->vertex_list[0].st;
  const void *color_ptr = impl_->vertex_list[0].rgba;
  srglVertexAttribPointer(position_location, 3, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), position_ptr);
  srglVertexAttribPointer(texcoord_location, 2, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), texcoord_ptr);
  srglVertexAttribPointer(color_location, 4, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), color_ptr);

  // default set
  mat4 identity;
  SetIdentity(&identity);
  srglUniformMatrix4fv(projection_location, 1, GL_FALSE, identity.value);
  srglUniformMatrix4fv(modelview_location, 1, GL_FALSE, identity.value);

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

void ImmediateModeEmulator::Color4f(float r, float g, float b, float a) {
  ImmediateModeEmulatorImpl *renderer = impl();
  renderer->color[0] = r;
  renderer->color[1] = g;
  renderer->color[2] = b;
  renderer->color[3] = a;
}
}