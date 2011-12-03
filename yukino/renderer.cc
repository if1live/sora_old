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
  uniform mat4 u_mvp; \
attribute vec3 a_position;  \
attribute vec2 a_texcoord;    \
varying vec2 v_texcoord;  \
void main() { \
	v_texcoord = a_texcoord;  \
  gl_Position = u_mvp * vec4(a_position, 0.5);   \
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
  glUseProgram(prog.prog);
  GLint mvp_location = prog.GetUniformLocation("u_mvp");
  GLint texcoord_location = prog.GetAttribLocation("a_texcoord");
  GLint position_location = prog.GetAttribLocation("a_position");
  SR_ASSERT(mvp_location != -1);
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(position_location != -1);
  
  glEnableVertexAttribArray(texcoord_location);
  glEnableVertexAttribArray(position_location);
  
  const void *position_ptr = impl_->vertex_list[0].xyz;
  const void *texcoord_ptr = impl_->vertex_list[0].st;
  glVertexAttribPointer(position_location, 3, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), position_ptr);
  glVertexAttribPointer(texcoord_location, 2, GL_FLOAT,
    GL_FALSE, sizeof(Vertex), texcoord_ptr);

  //test mvp
  using sora::mat4;
  mat4 mvp;
  sora::SetIdentity(&mvp);
  sora::SetScale(0.5f, &mvp);

  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp.value);
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

  RendererPimpl *renderer = Renderer::GetInstance().impl_;
  if (renderer->mode == GL_QUADS) {
    glDrawElements(GL_TRIANGLES, 
      renderer->curr_index * 3 / 2,
      GL_UNSIGNED_SHORT, 
      renderer->index_list);
  } else {
    glDrawArrays(renderer->mode, 0, renderer->curr_index);
  }
}
}