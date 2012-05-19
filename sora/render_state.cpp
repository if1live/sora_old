﻿/*  Copyright (C) 2011-2012 by if1live */
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
#include "render_state.h"
#include "gl_render_state.h"

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"

#include "matrix_stack.h"
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;

namespace sora {;
RenderState::RenderState()
: policy_(nullptr),
win_width_(640),
win_height_(480),
model_mat_stack_(new MatrixStack()), 
projection_mat_(glm::mat4(1.0f)),
view_mat_(glm::mat4(1.0f)) {
  policy_ = new sora::gl::GLRenderState(this);
}

RenderState::~RenderState() {
  if(policy_ != nullptr) {
    delete(policy_);
    policy_ = nullptr;
  }
}


void RenderState::UseShader(Shader &shader) { 
  policy_->UseShader(shader); 
}

void RenderState::UseTexture(Texture &tex, int unit) {
  policy_->UseTexture(tex, unit);
}
void RenderState::UnuseTexture(int unit) {
  policy_->UnuseTexture(unit);
}
void RenderState::UseMaterial(const Material &mtl) {
  policy_->UseMaterial(mtl);
}
void RenderState::UnuseMaterial() {
  policy_->UnuseMaterial();
}
const Material &RenderState::LastMaterial() const {
  return policy_->LastMaterial();
}
void RenderState::Set2D() {
  if(render_state_ != kRenderState2D) {
    policy_->Set2D(); 
    render_state_ = kRenderState2D;
  }
}
void RenderState::Set3D() { 
  if(render_state_ != kRenderState3D) {
    policy_->Set3D(); 
    render_state_ = kRenderState3D;
  }
}

void RenderState::EndRender() {
  policy_->EndRender(); 
}
void RenderState::SetWinSize(int width, int height) { 
  if(win_width_ != width || win_height_ != height) {
    policy_->SetWinSize(width, height); 
    win_width_ = width;
    win_height_ = height;
  }
}

void RenderState::ResetAllMatrix() {
  ResetProjectionMat();
  ResetViewMat();
  ResetModelMat();
}
void RenderState::ResetProjectionMat() {
  projection_mat_ = glm::mat4(1.0f);
}
void RenderState::ResetViewMat() {
  view_mat_ = glm::mat4(1.0f);
}
void RenderState::ResetModelMat() {
  model_mat_stack_->Clear();
}
const glm::mat4 &RenderState::model_mat() const {
  return model_mat_stack_->Top();
}

void RenderState::ClearBuffer(bool color, bool depth, bool stencil, const sora::vec4ub &value) {
  policy_->ClearBuffer(color, depth, stencil, value);  
}
glm::mat4 RenderState::GetMVPMatrix() const {
  mat4 mvp(1.0f);
  mvp *= projection_mat();
  mvp *= view_mat();
  mvp *= model_mat();
  return mvp;
}

} //namespace sora