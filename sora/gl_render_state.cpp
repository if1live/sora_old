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
#include "gl_render_state.h"

#if SR_USE_GL

#include "gl_env.h"
#include "renderer_env.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "shader.h"
#include "texture.h"

namespace sora {;
namespace gl {
  GLRenderState::GLRenderState(RenderState *state)
    : state_(state) {
      EndRender();
  }
  GLRenderState::~GLRenderState() {

  }
  void GLRenderState::SetWinSize(int w, int h) {
    LOGI("setupGraphics(%d, %d)", (int)w, (int)h);

    glViewport(0, 0, (int)w, (int)h);
    SR_CHECK_ERROR("glViewport");
  }
  void GLRenderState::EndRender() {
    last_prog_id_ = 0;
    last_tex_id_ = 0;
  }
  void GLRenderState::UseShader(Shader &shader) {
    ShaderHandleType handle = shader.handle();
    if(last_prog_id_ != handle) {
      glUseProgram(handle);
      last_prog_id_ = handle;
    }
  }
  void GLRenderState::UseTexture(Texture &tex) {
    TextureHandleType handle = tex.handle();
    if(last_tex_id_ != handle) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, handle);
      last_tex_id_ = handle;
    }
  }
  void GLRenderState::Set2D() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float win_w = (float)state_->win_width();
    float win_h = (float)state_->win_height();
    glm::mat4 projection = glm::ortho(0.0f, win_w, 0.0f, win_h);
    state_->set_projection_mat(projection);

  }
  void GLRenderState::Set3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_BLEND);

    //projection 행렬 설정
    float win_w = (float)state_->win_width();
    float win_h = (float)state_->win_height();
    glm::mat4 projection = glm::perspective(45.0f, win_w/ win_h, 0.1f, 100.0f);
    state_->set_projection_mat(projection);
  }
} //namespace gl
} //namespace sora

#endif  // SR_USE_GL