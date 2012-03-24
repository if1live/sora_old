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
#include "renderer/matrix_stack.h"
#include <glm/gtc/matrix_transform.hpp>

TEST(MatrixStack, Push_Pop) {
  using sora::MatrixStack;
  using glm::mat4;

  MatrixStack mat_stack;

  //최초에는 초기행렬 존재하도록했다
  EXPECT_EQ(1, mat_stack.Size());

  //맨 밑은 항상 단위행렬로 유지되도록함
  mat_stack.Pop();
  EXPECT_EQ(1, mat_stack.Size());
  EXPECT_EQ(mat_stack.Top(), glm::mat4(1.0f));

  mat_stack.Scale(1, 2, 3);
  EXPECT_EQ(1, mat_stack.Size());
  EXPECT_EQ(mat_stack.Top(), glm::scale(glm::mat4(1.0f), glm::vec3(1, 2, 3)));

  mat_stack.Push();
  EXPECT_EQ(2, mat_stack.Size());
  EXPECT_EQ(mat_stack.Top(), glm::scale(glm::mat4(1.0f), glm::vec3(1, 2, 3)));

  mat_stack.Translate(1, 2, 3);
  EXPECT_NE(mat_stack.Top(), glm::scale(glm::mat4(1.0f), glm::vec3(1, 2, 3)));

  mat_stack.Pop();
  EXPECT_EQ(1, mat_stack.Size());
  EXPECT_EQ(mat_stack.Top(), glm::scale(glm::mat4(1.0f), glm::vec3(1, 2, 3)));
}