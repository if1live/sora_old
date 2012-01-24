// Copyright (C) 2011 by if1live
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
#include "sora/math_helper.h"

using namespace sora;

TEST(MathHelper, IsIdentity) {
  using namespace sora;
  f32 data1[] = {1, 0, 0, 1};
  mat2 m1(data1);
  EXPECT_EQ(true, sora::IsIdentity(m1));

  f32 data2[] = {1, 0, 1, 2};
  mat2 m2(data2);
  EXPECT_EQ(false, sora::IsIdentity(m2));
}

TEST(MathHelper, SetIdentity) {
  using namespace sora;
  mat2 m1;
  EXPECT_EQ(false, sora::IsIdentity(m1));

  SetIdentity(&m1);
  EXPECT_EQ(true, sora::IsIdentity(m1));
}


TEST(MTMath, degree2radian) {
	//180degree = pi rad
	EXPECT_TRUE(EqualAbsError(kPi, DegreeToRadian(180.0f), 0.1f));
	//90degree = 1/2 pi rad
	EXPECT_TRUE(EqualAbsError(kPi/2, DegreeToRadian(90.0f), 0.1f));
}

TEST(MTMath, radian2degree)
{
	//pi rad = 180degree
	EXPECT_TRUE(EqualAbsError(180.0f, RadianToDegree(kPi), 0.1f));
	//1/2pi rad = 90degree
	EXPECT_TRUE(EqualAbsError(90.0f, RadianToDegree(kPi/2), 0.1f));
}

TEST(MTMath, isPower) {
	//int test
	EXPECT_EQ(true, IsPower(2, 4));
	EXPECT_EQ(false, IsPower(2, 5));
	//double
	EXPECT_EQ(true, IsPower(2.0, 4.0));
	EXPECT_EQ(true, IsPower(3.0, 9.0));
	EXPECT_EQ(false, IsPower(3.0, 9.1));
	//float
	EXPECT_EQ(true, IsPower(2.0f, 4.0f));
	EXPECT_EQ(true, IsPower(3.0f, 9.0f));
	EXPECT_EQ(false, IsPower(3.0f, 9.1f));
}

TEST(MTMath, ceilPower)
{
	//int test
	EXPECT_EQ(4, CeilPower(2, 4));
	EXPECT_EQ(8, CeilPower(2, 5));
	//double
	EXPECT_EQ(4.0, CeilPower(2.0, 4.0));
	EXPECT_EQ(9.0, CeilPower(3.0, 9.0));
	EXPECT_EQ(27.0, CeilPower(3.0, 9.1));
}

TEST(MTMath, isNaN) {
	//number
	EXPECT_EQ(false, IsNaN(static_cast<int>(1)));
	EXPECT_EQ(false, IsNaN(static_cast<float>(1)));
	EXPECT_EQ(false, IsNaN(static_cast<double>(1)));
	EXPECT_EQ(false, IsNaN(static_cast<char>(1)));
}

TEST(MTMath, isInf) {
	//number
	EXPECT_EQ(false, IsInf(static_cast<int>(1)));
	EXPECT_EQ(false, IsInf(static_cast<float>(1)));
	EXPECT_EQ(false, IsInf(static_cast<double>(1)));
	EXPECT_EQ(false, IsInf(static_cast<char>(1)));

	//inf
	float divider = 0;
	float nan = 1.0 / divider;
	EXPECT_EQ(true, IsInf(nan));
}

TEST(MTMath, IsNormalNumber) {
	EXPECT_EQ(true, IsNormalNumber(static_cast<int>(1)));
	EXPECT_EQ(true, IsNormalNumber(static_cast<float>(1)));
	
	//inf
	float divider = 0;
	float inf = 1.0 / divider;
	EXPECT_EQ(false, IsNormalNumber(inf));
}


TEST(MTMath, calcIncludeAngleRadian) {
	float radian1 = 0.0067581712f;
	float radian2 = 0.0066478122f;
	CalcIncludeAngleRadian(radian1, radian2);
}

TEST(MTMath, lerp) {
	//float
	float f1 = 10;
	float f2 = 100;
	EXPECT_EQ(10, Lerp(f1, f2, 0));
	EXPECT_EQ(100, Lerp(f1, f2, 1));
	EXPECT_EQ(55, Lerp(f1, f2, 0.5));
	
	//vector
	vec2 v1(1, 1);
	vec2 v2(100, 20);
	EXPECT_TRUE(v1 == Lerp(v1, v2, 0));
	EXPECT_TRUE(v2 == Lerp(v1, v2, 1));
	EXPECT_TRUE(vec2((1+100)/2.0, (1+20)/2.0) == Lerp(v1, v2, 0.5));
}