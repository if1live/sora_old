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
#include "gl_render_device.h"

#include "gl_env.h"
#include "renderer/renderer_env.h"
#include "gl_shader.h"
#include "gl_texture.h"

namespace sora {;
namespace gl {
  GLRenderDevice::GLRenderDevice(Device *dev)
  : win_width_(640),
  win_height_(480),
  last_prog_id_(-1),
  last_tex_id_(-1) {

  }
  GLRenderDevice::~GLRenderDevice() {

  }
  void GLRenderDevice::SetWinSize(int w, int h) {
    if(win_width_ != w || win_height_ != h) {
      LOGI("setupGraphics(%d, %d)", (int)w, (int)h);
      win_width_ = w;
      win_height_ = h;

      glViewport(0, 0, (int)w, (int)h);
      SR_CHECK_ERROR("glViewport");
    }
  }
  void GLRenderDevice::EndRender() {
    last_prog_id_ = -1;
    last_tex_id_ = -1;
  }
  void GLRenderDevice::UseShader(GLProgram &prog) {
    if(last_prog_id_ != prog.prog) {
      glUseProgram(prog.prog);
      last_prog_id_ = prog.prog;
    }
  }
  void GLRenderDevice::UseTexture(GLTexture &tex) {
    if(last_tex_id_ != tex.handle()) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex.handle());
      last_tex_id_ = tex.handle();
    }
  }
  void GLRenderDevice::Set2D() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  void GLRenderDevice::Set3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_BLEND);
  }
} //namespace gl
} //namespace sora