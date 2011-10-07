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
#include "precompile.h"
#include "runa/basic_texture_shader.h"
#include "runa/shader_program.h"

using runa::ShaderProgram;

namespace runa {;

const char texture_vertex_src[] = "uniform mat4 u_mvp;  "
    "attribute vec4 a_position; "
    "attribute vec2 a_texcoord; "
    "uniform vec4 u_color;  "    
    "varying vec4 v_color;  "
    "varying vec2 v_texcoord; "
    "void main()  "
    "{  "
    "v_texcoord = a_texcoord; "
    "v_color = u_color; "
    "gl_Position = u_mvp * a_position;"
    "}";
const char texture_fragment_src[] = ""
    "precision mediump float;  "
    "varying vec4 v_color;  "
    "varying vec2 v_texcoord; "
    "uniform sampler2D s_texture; "
    "void main()  "
    "{  "
    "vec4 tmp = texture2D(s_texture, v_texcoord); "
    "vec4 tmp1 = vec4(tmp.x * v_color.x, tmp.y * v_color.y, tmp.z * v_color.z, tmp.w * v_color.w);"
    "gl_FragColor = tmp1;  "
    "}  ";

BasicTextureShader::BasicTextureShader()
  : position_location_(-1),
color_location_(-1),
texcoord_location_(-1),
mvp_location_(-1) {
}
BasicTextureShader::~BasicTextureShader() {
}

void BasicTextureShader::Initialize() {
  SR_ASSERT(program_.get() == NULL);
  program_.reset(new ShaderProgram(texture_vertex_src, texture_fragment_src));

  position_location_ = program_->GetAttribLocation("a_position");
  color_location_ = program_->GetUniformLocation("u_color");
  texcoord_location_ = program_->GetAttribLocation("a_texcoord");
  mvp_location_ = program_->GetUniformLocation("u_mvp");
  SR_ASSERT(position_location_ != -1);
  SR_ASSERT(color_location_ != -1);
  SR_ASSERT(texcoord_location_ != -1);
  SR_ASSERT(mvp_location_ != -1);
}
bool BasicTextureShader::IsInitialized() const {
  if(program_.get() == NULL) {
    return false;
  } else {
    return true;
  }
}
const runa::ShaderProgram &BasicTextureShader::program() const {
  return *program_;
}
runa::ShaderProgram &BasicTextureShader::program() {
  return *program_;
}

void BasicTextureShader::SetColorToWhite() {
  float color[] = {1, 1, 1, 1};
  SetColor4fv(color);
}
void BasicTextureShader::SetColor4fv(const float *ptr) {
  glUniform4fv(color_location_, 1, ptr);
}
void BasicTextureShader::SetMatrix(const float *m) {
  glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, m);
}

int BasicTextureShader::position_location() const {
  return position_location_;
}
int BasicTextureShader::texcoord_location() const {
  return texcoord_location_;
}
void BasicTextureShader::Use() {
  program_->Use();
}
}
