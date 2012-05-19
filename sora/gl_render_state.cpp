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
#include "material.h"

namespace sora {;
namespace gl {
  GLRenderState::GLRenderState(RenderState *state)
    : state_(state), last_mtl_(new Material()) {
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
    *last_mtl_ = Material();
    {
      auto it = last_tex_id_list_.begin();
      auto endit = last_tex_id_list_.end();
      for( ; it != endit ; ++it) {
        *it = 0;
      }
    }
  }
  void GLRenderState::UseShader(Shader &shader) {
    ShaderHandleType handle = shader.handle();
    if(last_prog_id_ != handle) {
      glUseProgram(handle);
      last_prog_id_ = handle;
    }
  }
  void GLRenderState::UnuseTexture(int unit) {
    GLenum unit_code = GL_TEXTURE0 + unit;
    glActiveTexture(unit_code);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  void GLRenderState::UseTexture(Texture &tex, int unit) {
    TextureHandleType handle = tex.handle();
    if(last_tex_id_list_[unit] != handle) {
      GLenum unit_code = GL_TEXTURE0 + unit;
      glActiveTexture(unit_code);
      glBindTexture(GL_TEXTURE_2D, handle);
      last_tex_id_list_[unit] = handle;
    }
  }
  void GLRenderState::Set2D() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state_->set_projection_mat(GetProjection2D());

  }
  void GLRenderState::Set3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_BLEND);

    //projection 행렬 설정
    state_->set_projection_mat(GetProjection3D());
  }

  glm::mat4 GLRenderState::GetProjection3D() const {
    float win_w = (float)state_->win_width();
    float win_h = (float)state_->win_height();
    glm::mat4 projection = glm::perspective(45.0f, win_w/ win_h, 0.1f, 100.0f);
    return projection;
  }
  glm::mat4 GLRenderState::GetProjection2D() const {
    float win_w = (float)state_->win_width();
    float win_h = (float)state_->win_height();
    glm::mat4 projection = glm::ortho(0.0f, win_w, 0.0f, win_h);
    return projection;
    
  }
  void GLRenderState::ClearBuffer(bool color, bool depth, bool stencil, const sora::vec4ub &value) {
    unsigned int flag = 0;
    if(color) {
      flag |= GL_COLOR_BUFFER_BIT;
      float r = value.r / 255.0f;
      float g = value.g / 255.0f;
      float b = value.b / 255.0f;
      float a = value.a / 255.0f;
      glClearColor(r, g, b, a);
    }
    if(depth) {
      flag |= GL_DEPTH_BUFFER_BIT;
    }
    if(stencil) {
      flag |= GL_STENCIL_BUFFER_BIT;
    }
    if(flag) {
      glClear(flag);
    }
  }
  void GLRenderState::UseMaterial(const Material &mtl) {
    if(mtl == *last_mtl_) {
      return;
    }
    *last_mtl_ = mtl;
  }
  void GLRenderState::UnuseMaterial() {
    *last_mtl_ = Material();
  }
  const Material &GLRenderState::LastMaterial() const {
    return *last_mtl_;
  }
} //namespace gl
} //namespace sora

#endif  // SR_USE_GL