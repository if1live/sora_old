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
#include "matrix_stack.h"
#include "math_helper.h"

namespace sora {;
MatrixStack::MatrixStack()
: matrix_mode_(kProjection){
  // matrix stack를 적절히 초기화
  SetIdentity(&projection_mat_);
  SetIdentity(&modelview_mat_);
}
MatrixStack::~MatrixStack() {
}
MatrixStack &MatrixStack::GetInstance() {
  static MatrixStack ms;
  return ms;
}

mat4 &MatrixStack::GetCurrMatrix() {
  if (matrix_mode_ == kProjection) {
    return projection_mat_;
  } else {
    return modelview_mat_;
  }
}


void MatrixStack::UseProjectionMatrixMode() {
  matrix_mode_ = kProjection;
}
void MatrixStack::UseModelviewMatrixMode() {
  matrix_mode_ = kModelview;
}
void MatrixStack::SetMatrixToIdentity() {
  mat4 &m = GetCurrMatrix();
  SetIdentity(&m);
}
void MatrixStack::PushMatrix() {
  mat4 &m = GetCurrMatrix();
  matrix_stack_.push_back(m);
}
void MatrixStack::PopMatrix() {
  SR_ASSERT(matrix_stack_.empty() == false);
  mat4 &back = matrix_stack_.back();
  matrix_stack_.pop_back();
  mat4 &m = GetCurrMatrix();
  m = back;
}
void MatrixStack::Scale(float x, float y, float z) {
  mat4 scale;
  SetScale(x, y, z, &scale);
  MultMatrix(scale.value);
}
void MatrixStack::Translate(float x, float y, float z) {
  mat4 translate;
  SetTranslate(x, y, z, &translate);
  MultMatrix(translate.value);
}
void MatrixStack::Rotate(float degree, float x, float y, float z) {
  mat4 rotate;
  if (x == 0 && y == 0 && z == 1) {
    SetRotateZ(degree, &rotate);
  } else if (x == 0 && y == 1 && z == 0) {
    SetRotateY(degree, &rotate);
  } else if (x == 1 && y == 0 && z == 0) {
    SetRotateX(degree, &rotate);
  } else {
    SR_ASSERT(!"not support rotation");
  }
  MultMatrix(rotate.value);
}
void MatrixStack::MultMatrix(float *m) {
  mat4 &curr_mat = GetCurrMatrix();
  mat4 tmp(m);
  curr_mat *= tmp;
}
void MatrixStack::MatrixMode(int matrix_mode) {
  switch (matrix_mode) {
  case SR_PROJECTION:
#ifdef GL_PROJECTION
  case GL_PROJECTION:
#endif
    UseProjectionMatrixMode();
    break;
  case SR_MODELVIEW:
#ifdef GL_MODELVIEW
  case GL_MODELVIEW:
#endif
    UseModelviewMatrixMode();
    break;
  }
}

void MatrixStack::LookAt(float eye_x, float eye_y, float eye_z,
  float target_x, float target_y, float target_z,
  float up_x, float up_y, float up_z) {
  mat4 lookat;
  SetLookAt(eye_x, eye_y, eye_z,
    target_x, target_y, target_z,
    up_x, up_y, up_z, &lookat);
  mat4 &curr_mat = GetCurrMatrix();
  curr_mat *= lookat;
}
void MatrixStack::Perspective(float fovy, float aspect, float zNear, float zFar) {
  mat4 tmp;
  SetPerspective(fovy, aspect, zNear, zFar, &tmp);
  mat4 &curr_mat = GetCurrMatrix();
  curr_mat *= tmp;
}

}