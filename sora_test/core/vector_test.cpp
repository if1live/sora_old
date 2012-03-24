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
#include "sora/core/vector.h"

TEST(Vector, Vec_Set) {
  using namespace sora;
  vec2f_t v1;
  VecSet(1, 2, v1);
  EXPECT_EQ(1, v1[0]);
  EXPECT_EQ(2, v1[1]);

  vec3f_t v2;
  VecSet(1, 2, 3, v2);
  EXPECT_EQ(1, v2[0]);
  EXPECT_EQ(2, v2[1]);
  EXPECT_EQ(3, v2[2]);
}

TEST(Vector, Vec_SetZero) {
  using namespace sora;
  vec2f_t v1;
  VecSetZero(v1);
  EXPECT_EQ(0, v1[0]);
  EXPECT_EQ(0, v1[1]);

  vec3f_t v2;
  VecSetZero(v2);
  EXPECT_EQ(0, v2[0]);
  EXPECT_EQ(0, v2[1]);
  EXPECT_EQ(0, v2[2]);
}

TEST(Vector, Vec_Add) {
  using namespace sora;
  vec2f_t v1;
  vec2f_t v2;
  VecSet(1, 2, v1);
  VecSet(3, 4, v2);

  vec2f_t v3;
  VecAdd(v1, v2, v3);
  EXPECT_EQ(1 + 3, v3[0]);
  EXPECT_EQ(2 + 4, v3[1]);
}

TEST(Vector, Vec_Dot) {
  using namespace sora;
  vec2f_t v1;
  vec2f_t v2;
  VecSet(1, 2, v1);
  VecSet(3, 4, v2);
  float dot = VecDot(v1, v2);
  EXPECT_EQ(1*3 + 2*4, dot);
}

TEST(Vector, Vec_Mul) {
  using namespace sora;
  vec2f_t v1;
  VecSet(1, 2, v1);

  vec2f_t v2;
  VecMul(v1, 2.0f, v2);
  EXPECT_EQ(2, v2[0]);
  EXPECT_EQ(4, v2[1]);
}

TEST(Vector, Vec_Div) {
  using namespace sora;
  vec2f_t v1;
  VecSet(1, 2, v1);

  vec2f_t v2;
  VecDiv(v1, 2.0f, v2);
  EXPECT_EQ(0.5, v2[0]);
  EXPECT_EQ(1, v2[1]);
}

TEST(Vector, Vec_DivCheck) {
  using namespace sora;
  vec2f_t v1;
  VecSet(1, 2, v1);

  vec2f_t v2;
  int retcode = VecDivCheck(v1, 2.0f, v2);
  EXPECT_EQ(0, retcode);
  EXPECT_EQ(0.5, v2[0]);
  EXPECT_EQ(1, v2[1]);

  //divide by zero
  vec2f_t v3;
  retcode = VecDivCheck(v1, 0.0f, v3);
  EXPECT_EQ(-1, retcode);
}

TEST(Vector, VecEq_VecNotEq) {
  using namespace sora;
  vec2f_t v1;
  VecSet(1, 1, v1);
  vec2f_t v2;
  VecSet(1, 1, v2);
  vec2f_t v3;
  VecSet(1, 2, v3);
  EXPECT_EQ(true, VecEq(v1, v2));
  EXPECT_EQ(true, VecNotEq(v1, v3));
}

TEST(Vector, VecLength) {
  using namespace sora;
  vec2f_t v;
  VecSet(3, 4, v);
  EXPECT_EQ(25, VecLengthSquare(v));
  EXPECT_EQ(5, VecLength(v));
}

TEST(Vector, VecNormalized) {
  using namespace sora;
  vec2f_t v1;
  VecSet(3, 4, v1);
  vec2f_t v2;
  VecCopy(v1, v2);
  vec2f_t v3;
  VecSet(3.0f/5.0f, 4.0f/5.0f, v3);

  VecNormalized(v2);
  EXPECT_EQ(true, VecEq(v3, v2));

  vec2f_t normal_result;
  VecNormalize(v1, normal_result);
  EXPECT_EQ(true, VecEq(v2, normal_result));
}


TEST(Vector2, VecCross) {
  using namespace sora;
  vec3f_t xVec;
  VecSet(1, 0, 0, xVec);
	vec3f_t yVec;
  VecSet(0, 1, 0, yVec);
	vec3f_t result;
  VecCross(xVec, yVec, result);

	EXPECT_EQ(0, result[0]);
	EXPECT_EQ(0, result[1]);
	EXPECT_EQ(1, result[2]);
}

TEST(VectorTpl, Add) {
  using namespace sora;
  Vec2f v1;
  v1.x = 1; v1.y = 2;
  Vec2f v2;
  v2.x = 3; v2.y = 4;

  Vec2f v3 = v1;
  v3 += v2;
  EXPECT_EQ(4, v3.x);
  EXPECT_EQ(6, v3.y);

  v3 = v1 + v2;
  EXPECT_EQ(4, v3.x);
  EXPECT_EQ(6, v3.y);
}

TEST(VectorTpl, Length) {
  using namespace sora;
  Vec2f v2(3, 4);
  EXPECT_EQ(25, v2.LengthSquare());
  EXPECT_EQ(5, v2.Length());
}

TEST(VectorTpl, Normalized) {
  using namespace sora;
  Vec2f v2(3, 4);
  Vec2f v3 = v2;
  v3.Normalized();
  EXPECT_EQ(3.0f/5, v3.x);
  EXPECT_EQ(4.0f/5, v3.y);

  v3 = v2.Normalize();
  EXPECT_EQ(3.0f/5, v3.x);
  EXPECT_EQ(4.0f/5, v3.y);
}

TEST(VectorTpl, Minus) {
  using namespace sora;
  Vec2f v2(3, 4);
  Vec2f v3 = -v2;
  EXPECT_EQ(-3, v3.x);
  EXPECT_EQ(-4, v3.y);
}


TEST(VectorTpl, VecEq_VecNotEq) {
  using namespace sora;
  Vec2f v1(1, 1);
  Vec2f v2(1, 1);
  Vec2f v3(1, 2);
  
  EXPECT_EQ(true, v1 == v2);
  EXPECT_EQ(true, v1 != v3);
}