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
RenderStateInterface::RenderStateInterface()
: win_width_(640),
win_height_(480),
model_mat_stack_(new MatrixStack()), 
projection_mat_(glm::mat4(1.0f)),
view_mat_(glm::mat4(1.0f)) {
}

RenderStateInterface::~RenderStateInterface() {
}
RenderStateInterface *RenderStateInterface::Create() {
  return new sora::gl::GLRenderState();
}
void RenderStateInterface::ResetAllMatrix() {
  ResetProjectionMat();
  ResetViewMat();
  ResetModelMat();
}
void RenderStateInterface::ResetProjectionMat() {
  projection_mat_ = glm::mat4(1.0f);
}
void RenderStateInterface::ResetViewMat() {
  view_mat_ = glm::mat4(1.0f);
}
void RenderStateInterface::ResetModelMat() {
  model_mat_stack_->Clear();
}
const glm::mat4 &RenderStateInterface::model_mat() const {
  return model_mat_stack_->Top();
}

glm::mat4 RenderStateInterface::GetMVPMatrix() const {
  mat4 mvp(1.0f);
  mvp *= projection_mat();
  mvp *= view_mat();
  mvp *= model_mat();
  return mvp;
}

glm::mat4 RenderStateInterface::GetProjection3D() const {
  float win_w = (float)win_width();
  float win_h = (float)win_height();
  glm::mat4 projection = glm::perspective(45.0f, win_w/ win_h, 0.1f, 100.0f);
  return projection;
}
glm::mat4 RenderStateInterface::GetProjection2D() const {
  float win_w = (float)win_width();
  float win_h = (float)win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_w, 0.0f, win_h);
  return projection;
    
}
} //namespace sora