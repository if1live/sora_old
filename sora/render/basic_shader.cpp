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
#include "basic_shader.h"

#include "shader.h"
#include "gl_inc.h"

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

struct BasicShaderImpl {
  BasicShaderImpl(BasicShader *clazz) {
    // initialize program
    prog.Init(vert_src, frag_src);

    // connect location
    clazz->modelview_location = prog.GetUniformLocation("u_modelview");
    clazz->projection_location = prog.GetUniformLocation("u_projection");
    SR_ASSERT(clazz->modelview_location != -1);
    SR_ASSERT(clazz->projection_location != -1);

    clazz->texcoord_location = prog.GetAttribLocation("a_texcoord");
    clazz->position_location = prog.GetAttribLocation("a_position");
    clazz->color_location = prog.GetAttribLocation("a_color");
    SR_ASSERT(clazz->texcoord_location != -1);
    SR_ASSERT(clazz->position_location != -1);
    SR_ASSERT(clazz->color_location != -1);
  }
  Program prog;
};

BasicShader::BasicShader() 
: impl_(NULL),
modelview_location(-1),
projection_location(-1),
texcoord_location(-1),
color_location(-1),
position_location(-1) {
  impl_ = new BasicShaderImpl(this);
}
BasicShader::~BasicShader() {
  if (impl_ != NULL) {
    delete(impl_);
    impl_ = NULL;
  }
}

void BasicShader::Init() {
}
void BasicShader::Use() {
  Program &prog = impl_->prog;
  srglUseProgram(prog.prog);
}
}