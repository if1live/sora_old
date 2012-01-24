/* Copyright (C) 2011 by if1live */
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
#include "sora/vector.h"

TEST(Vector2, EQ_NEQ) {
  sora::vec2 v1(1, 1);
  sora::vec2 v2(1, 1);
  sora::vec2 v3(1, 2);
  EXPECT_EQ(true, v1 == v2);
  EXPECT_EQ(true, v1 != v3);
}

TEST(Vector2, Operator) {
  using namespace sora;
  {
    vec2 v1(1, 2);
    vec2 v2(2, 4);
    vec2 v3 = v1 + v1;
    EXPECT_EQ(true, v3 == v2);
  }
  {
    vec2 v1(1, 2);
    vec2 v2(2, 4);
    vec2 v3 = v2 - v1;
    EXPECT_EQ(true, v3 == v1);
  }
  {
    vec2 v1(1, 2);
    vec2 v2 = -v1;
    EXPECT_EQ(true, v2 == vec2(-1, -2));
  }
  {
    vec2 v1(1, 2);
    vec2 v2(2, 4);
    vec2 v3 = v1 * 2.0f;
    EXPECT_EQ(true, v3 == v2);
  }
  {
    vec2 v1(1, 2);
    vec2 v2(2, 4);
    vec2 v3 = 2.0f * v1;
    EXPECT_EQ(true, v3 == v2);
  }
  {
    vec2 v1(1, 2);
    vec2 v2(2, 4);
    vec2 v3 = v2 / 2.0f;
    EXPECT_EQ(true, v3 == v1);
  }
}

TEST(Vector2, Length) {
  using namespace sora;
  vec2 v(3, 4);
  EXPECT_EQ(25, VectorLengthSquare(v));
  EXPECT_EQ(5, VectorLength(v));
}

TEST(Vector2, Dot) {
  using namespace sora;
  vec2 v1(2, 3);
  vec2 v2(3, 4);
  EXPECT_EQ(2*3+3*4, VectorDot(v1, v2));
}

TEST(Vector2, Normalized) {
  using namespace sora;
  vec2 v1(3, 4);
  vec2 v2 = v1;
  vec2 v3(3.0f/5.0f, 4.0f/5.0f);
  VectorNormalized(v2);
  EXPECT_EQ(true, v3 == v2);
  EXPECT_EQ(v3, VectorNormalize(v1));
}
TEST(Vector3, Cross) {
  using namespace sora;
  vec3 xVec(1, 0, 0);
	vec3 yVec(0, 1, 0);
	vec3 result = VectorCross(xVec, yVec);
	EXPECT_EQ(0, result.x);
	EXPECT_EQ(0, result.y);
	EXPECT_EQ(1, result.z);
}