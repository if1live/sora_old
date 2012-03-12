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
#ifndef SORA_MATRIX_STACK_H_
#define SORA_MATRIX_STACK_H_

#include "matrix.h"

#if SR_USE_PCH == 0
#include <vector>
#endif

const int kMaxMatrixStackSize = 16;

namespace sora {;
//matrix stack은 projection, modelview마다 따로 관리되야한다
//그래서 행렬스택용으로 따로 만들고 이것을 여러개 두도록하자
class SR_DLL MatrixStack {
public:
  MatrixStack();
  ~MatrixStack();
  static MatrixStack &GetInstance();

public:
  // member method
  void SetMatrixToIdentity();
  void PushMatrix();
  void PopMatrix();
  int Size() { return stack_top_ + 1; } //비엇다가 -1이다

  void Scale(float x, float y, float z);
  void Translate(float x, float y, float z);
  void Rotate(float degree, float x, float y, float z);
  void MultMatrix(float *m);
 
  void LookAt(const Vec3f &eye, const Vec3f &target, const Vec3f &up);
  void Perspective(float fovy, float aspect, float zNear, float zFar);
  void Ortho(float left, float right, float bottom, float top, float near_val, float far_val);

  Mat4f &GetCurrMatrix() { return curr_mat_; }

private:
  Mat4f curr_mat_;

  //mat4f가 배열이라서 vector로 생성이 안된다
  //그래서 고정크기 배열로 스택을 따로 구성
  //matrix stack
  Mat4f matrix_stack_[kMaxMatrixStackSize + 1]; //overflow방지용
  int stack_top_; //가장 위에 존재하는 Mat4를 가리킨다. 비엇으면 -1
};
}


#endif  // SORA_MATRIX_STACK_H_