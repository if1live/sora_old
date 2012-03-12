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
: stack_top_(-1) {
  // matrix stack를 적절히 초기화
  MatSetIdentity(curr_mat_);
}
MatrixStack::~MatrixStack() {
}
MatrixStack &MatrixStack::GetInstance() {
  static MatrixStack ms;
  return ms;
}


void MatrixStack::SetMatrixToIdentity() {
  Mat4f &m = GetCurrMatrix();
  MatSetIdentity(m);
}
void MatrixStack::PushMatrix() {
  if(Size() >= kMaxMatrixStackSize) {
    SR_ASSERT("stack overflow");
    return;
  }
  Mat4f &m = GetCurrMatrix();
  stack_top_++;
  memcpy(matrix_stack_[stack_top_], m, sizeof(Mat4f));
}
void MatrixStack::PopMatrix() {
  if(Size() <= -1) {
    SR_ASSERT("stack underflow");
    return;
  }
  Mat4f back;
  memcpy(back, matrix_stack_[stack_top_], sizeof(Mat4));
  stack_top_--;
  Mat4f &m = GetCurrMatrix();
  MatCopy(back, m);
}
void MatrixStack::Scale(float x, float y, float z) {
  Mat4f scale;
  sora::Scale(x, y, z, scale);
  MultMatrix(scale);
}
void MatrixStack::Translate(float x, float y, float z) {
  Mat4f translate;
  sora::Translate(x, y, z, translate);
  MultMatrix(translate);
}
void MatrixStack::Rotate(float degree, float x, float y, float z) {
  Mat4f rotate;
  if (x == 0 && y == 0 && z == 1) {
    RotateZ(degree, rotate);
  } else if (x == 0 && y == 1 && z == 0) {
    RotateY(degree, rotate);
  } else if (x == 1 && y == 0 && z == 0) {
    RotateX(degree, rotate);
  } else {
    SR_ASSERT(!"not support rotation");
  }
  MultMatrix(rotate);
}
void MatrixStack::MultMatrix(float *m) {
  Mat4f &curr_mat = GetCurrMatrix();
  Mat4f tmp;
  memcpy(tmp, m, sizeof(tmp));
  MatMult(curr_mat, tmp, curr_mat);
}
void MatrixStack::LookAt(const Vec3f &eye, const Vec3f &target, const Vec3f &up) {
  Mat4f lookat;
  sora::LookAt(eye, target, up, lookat);
  Mat4f &curr_mat = GetCurrMatrix();
  MatMult(curr_mat, lookat, curr_mat);
}
void MatrixStack::Perspective(float fovy, float aspect, float zNear, float zFar) {
  Mat4f tmp;
  sora::Perspective(fovy, aspect, zNear, zFar, tmp);
  Mat4f &curr_mat = GetCurrMatrix();
  MatMult(curr_mat, tmp, curr_mat);
}
void MatrixStack::Ortho(float left, float right, float bottom, float top, float near_val, float far_val) {
  Mat4f tmp;
  sora::Ortho(left, right, bottom, top, near_val, far_val, tmp);
  Mat4f &curr_mat = GetCurrMatrix();
  MatMult(curr_mat, tmp, curr_mat);
}

}