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
#include "sora_test_stdafx.h"
#include "matrix_stack.h"

TEST(MatrixStack, Push_Pop) {
  using namespace sora;
  MatrixStack mat_stack;
  EXPECT_EQ(0, mat_stack.Size());

  mat_stack.PushMatrix(); //identity
  EXPECT_EQ(1, mat_stack.Size());

  mat_stack.Scale(1, 2, 3);
  mat_stack.PushMatrix(); //scale
  EXPECT_EQ(2, mat_stack.Size());
  
  Mat4f m1;
  sora::Scale(1.0f, 2.0f, 3.0f, m1);
  EXPECT_EQ(true, MatEq(m1, mat_stack.GetCurrMatrix()));
  
  mat_stack.PopMatrix();  //scale
  EXPECT_EQ(true, MatEq(m1, mat_stack.GetCurrMatrix()));
  EXPECT_EQ(1, mat_stack.Size());

  mat_stack.PopMatrix();  //identity
  Mat4f m2;
  sora::MatSetIdentity(m2);
  EXPECT_EQ(true, MatEq(m2, mat_stack.GetCurrMatrix()));
  EXPECT_EQ(0, mat_stack.Size());
}