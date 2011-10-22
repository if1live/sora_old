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
#ifndef RENDERING_RUNA_MATRIX_STACK_H_
#define RENDERING_RUNA_MATRIX_STACK_H_

#include <vector>
#include "matsu/matrix.h"

namespace runa {;
class MatrixStack {
public:
  MatrixStack();
  ~MatrixStack();
  // stack operator
  void Push(const matsu::mat4 &m);
  matsu::mat4 Pop();
  bool IsEmpty() const;
  void Clear();

  // gl verstion interface
  const matsu::mat4 &curr_matrix() const;
  const float *Pointer() const;
  void ToIdentity();
  void Translate(float x, float y, float z);
  void Scale(float x, float y, float z);
  void Rotate(float degree, float x, float y, float z);
  void MultiplyMatrix(const matsu::mat4 &m);
private:
  std::vector<matsu::mat4> stack_;
  matsu::mat4 curr_matrix_;
};
}

#endif  // RENDERING_RUNA_MATRIX_STACK_H_
