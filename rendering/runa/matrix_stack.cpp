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
#include "precompile.h"
#include "runa/matrix_stack.h"
#include "matsu/matrix.h"

using matsu::mat4;
using matsu::Matrix;

namespace runa {;
MatrixStack::MatrixStack() {
  ToIdentity();
}
MatrixStack::~MatrixStack() {
}
// stack operator
void MatrixStack::Push(const matsu::mat4 &m) {
  stack_.push_back(m);
}
matsu::mat4 MatrixStack::Pop() {
  mat4 result = stack_.back();
  stack_.pop_back();
  return result;
}
bool MatrixStack::IsEmpty() const {
  return stack_.empty();
}
void MatrixStack::Clear() {
  stack_.clear();
}

// gl verstion interface
const matsu::mat4 &MatrixStack::curr_matrix() const {
  return curr_matrix_;
}
void MatrixStack::ToIdentity() {
  curr_matrix_ = matsu::Matrix::Identity<float>();
}
void MatrixStack::Translate(float x, float y, float z) {
  mat4 m = matsu::Matrix::Translate<float>(x, y, z);
  MultiplyMatrix(m);
}
void MatrixStack::Scale(float x, float y, float z) {
  mat4 m = matsu::Matrix::Scale(x, y, z);
  MultiplyMatrix(m);
}
void MatrixStack::Rotate(float degree, float x, float y, float z) {
  if (x == 0 && y == 0) {
    // z axis
    mat4 m = Matrix::RotateZ(degree);
    MultiplyMatrix(m);
  } else if (x == 0 && z == 0) {
    // y axis
    mat4 m = Matrix::RotateY(degree);
    MultiplyMatrix(m);
  } else if (y == 0 && z == 0) {
    // x axis
    mat4 m = Matrix::RotateX(degree);
    MultiplyMatrix(m);
  } else {
    SR_ASSERT(!"cant calc");
  }
}
void MatrixStack::MultiplyMatrix(const matsu::mat4 &m) {
  curr_matrix_ *= m;
}
const float *MatrixStack::Pointer() const {
  return curr_matrix_.Pointer();
}
}
