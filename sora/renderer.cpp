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
#include "renderer.h"

#include "texture.h"
#include "gl_helper.h"
#include "shader.h"

namespace sora {;
Renderer::Renderer() 
: win_width_(480), 
win_height_(320),
last_tex_id_(-1) {
}
Renderer::~Renderer() {
}

void Renderer::SetWindowSize(float w, float h) {
  LOGI("setupGraphics(%d, %d)", (int)w, (int)h);

  win_width_ = w;
  win_height_ = h;

  LOGI("Version : %s", GLHelper::GetVersion().c_str());
  LOGI("Vendor : %s", GLHelper::GetVender().c_str());
  LOGI("Renderer : %s", GLHelper::GetRenderer().c_str());
  LOGI("Extensions : %s", GLHelper::GetExtensions().c_str());

  glViewport(0, 0, (int)w, (int)h);
  GLHelper::CheckError("glViewport");
}

void Renderer::Set2D() {
  glDisable(GL_DEPTH_TEST);
}

void Renderer::Set3D() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

void Renderer::SetTexture(const Texture &tex) {
  if(last_tex_id_ != tex.handle()) {
    glBindTexture(GL_TEXTURE_2D, tex.handle());
    last_tex_id_ = tex.handle();
  }
}

void Renderer::SetShader(const ShaderProgram &prog) {
  glUseProgram(prog.prog);
}

void Renderer::EndRender() {
  last_tex_id_ = -1;

  GLHelper::CheckError("EndRender");
}
}
