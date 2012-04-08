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
#include "sora_test_stdafx.h"
#include "renderer/gl_helper.h"

using namespace std;
using namespace sora;
using namespace glm;

TEST(GLHelper, TypeToGLEnum) {
  EXPECT_EQ(GL_FLOAT, GLHelper::TypeToGLEnum<float>());
  EXPECT_EQ(GL_UNSIGNED_BYTE, GLHelper::TypeToGLEnum<unsigned char>());
  EXPECT_EQ(GL_BYTE, GLHelper::TypeToGLEnum<char>());
  EXPECT_EQ(GL_SHORT, GLHelper::TypeToGLEnum<short>());
  EXPECT_EQ(GL_UNSIGNED_SHORT, GLHelper::TypeToGLEnum<unsigned short>());
  EXPECT_EQ(GL_INT, GLHelper::TypeToGLEnum<int>());
  EXPECT_EQ(GL_UNSIGNED_INT, GLHelper::TypeToGLEnum<unsigned int>());
}

TEST(GLHelper, VecToGLEnum) {
  EXPECT_EQ(GL_FLOAT, GLHelper::VecToGLEnum(vec3(0.0)));
  EXPECT_EQ(GL_INT, GLHelper::VecToGLEnum(ivec2(0.0)));
}