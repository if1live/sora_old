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
#include "runa/basic_color_shader.h"
#include "runa/shader_program.h"

using runa::ShaderProgram;

namespace runa {;
const char basic_vertex_src[] = "uniform mat4 u_mvp;  "
    "attribute vec4 a_position;  "
    "uniform vec4 u_color;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "v_color = u_color; "
    "gl_Position = u_mvp * a_position;"
    "}";
const char basic_fragment_src[] = ""
    "precision mediump float;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "gl_FragColor = v_color;  "
    "}  ";

BasicColorShader::BasicColorShader() 
: color_location_(-1),
position_location_(-1),
mvp_location_(-1) {
}
BasicColorShader::~BasicColorShader() {
}
void BasicColorShader::Initialize() {
  if(program_.get() == NULL) {
    program_.reset(new ShaderProgram(basic_vertex_src, basic_fragment_src));

    position_location_ = program_->GetAttribLocation("a_position");
    color_location_ = program_->GetUniformLocation("u_color");
    mvp_location_ = program_->GetUniformLocation("u_mvp");
    SR_ASSERT(position_location_ != -1);
    SR_ASSERT(color_location_ != -1);
    SR_ASSERT(mvp_location_ != -1);
  }
}
void BasicColorShader::Use() {
  program_->Use();
}
const runa::ShaderProgram &BasicColorShader::get_program() const {
  return *program_;
}
runa::ShaderProgram &BasicColorShader::get_program() {
  return *program_;
}
int BasicColorShader::get_position_location() const {
  return position_location_;
}
int BasicColorShader::get_color_location() const {
  return color_location_;
}
int BasicColorShader::get_mvp_location() const {
  return mvp_location_;
}
}
