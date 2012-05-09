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
#include "render_device.h"
#include "gl_render_device.h"

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"

#include "matrix_stack.h"
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;

namespace sora {;
RenderDevice::RenderDevice()
: policy_(nullptr),
win_width_(640),
win_height_(480),
model_mat_stack_(new MatrixStack()), 
projection_mat_(glm::mat4(1.0f)),
view_mat_(glm::mat4(1.0f)) {
  policy_ = new sora::gl::GLRenderDevice();
}

RenderDevice::~RenderDevice() {
  if(policy_ != nullptr) {
    delete(policy_);
    policy_ = nullptr;
  }
}


void RenderDevice::UseShader(Shader &shader) { 
  policy_->UseShader(shader); 
}
void RenderDevice::UseTexture(Texture &tex) { 
  policy_->UseTexture(tex); 
}

void RenderDevice::Set2D() {
  policy_->Set2D(); 
}
void RenderDevice::Set3D() { 
  policy_->Set3D(); 
}

void RenderDevice::EndRender() {
  policy_->EndRender(); 
}
void RenderDevice::SetWinSize(int width, int height) { 
  if(win_width_ != width || win_height_ != height) {
    policy_->SetWinSize(width, height); 
    win_width_ = width;
    win_height_ = height;
  }
}

void RenderDevice::ResetAllMatrix() {
  ResetProjectionMat();
  ResetViewMat();
  ResetModelMat();
}
void RenderDevice::ResetProjectionMat() {
  projection_mat_ = glm::mat4(1.0f);
}
void RenderDevice::ResetViewMat() {
  view_mat_ = glm::mat4(1.0f);
}
void RenderDevice::ResetModelMat() {
  model_mat_stack_->Clear();
}
const glm::mat4 &RenderDevice::model_mat() const {
  return model_mat_stack_->Top();
}

glm::vec3 RenderDevice::view_up_vec() const {
  //y
  vec3 side;
  for(int i = 0 ; i < 3 ; i++) {
    side[i] = view_mat_[i][1];
  }
  return side;
}
glm::vec3 RenderDevice::view_dir_vec() const {
  vec3 side;
  for(int i = 0 ; i < 3 ; i++) {
    side[i] = -view_mat_[i][2];
  }
  return side;
}
glm::vec3 RenderDevice::view_side_vec() const {
  //x
  vec3 side;
  for(int i = 0 ; i < 3 ; i++) {
    side[i] = view_mat_[i][0];
  }
  return side;
}
glm::vec3 RenderDevice::view_pos() const {
  mat3 view3_mat;
  for(int i = 0 ; i < 3 ; i++) {
    for(int j = 0 ; j < 3 ; j++) {
      view3_mat[i][j] = view_mat_[i][j];
    }
  }
  mat3 view3_inv_mat = glm::inverse(view3_mat);
  vec3 last_vec;
  for(int i = 0 ; i < 3 ; i++) {
    last_vec[i] = view_mat_[3][i];  //view행렬의 다른것과 혼자 방향이 다르다
  }

  vec3 view_pos = view3_inv_mat * last_vec;
  return -view_pos;
}
} //namespace sora