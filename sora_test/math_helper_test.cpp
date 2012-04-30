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
#include "sora/math_helper.h"

using namespace sora;

TEST(MTMath, degree2radian) {
	//180degree = pi rad
	EXPECT_TRUE(EqualAbsError(kPi, DegToRad(180.0f), 0.1f));
	//90degree = 1/2 pi rad
	EXPECT_TRUE(EqualAbsError(kPi/2, DegToRad(90.0f), 0.1f));
}

TEST(MTMath, radian2degree)
{
	//pi rad = 180degree
	EXPECT_TRUE(EqualAbsError(180.0f, RadToDeg(kPi), 0.1f));
	//1/2pi rad = 90degree
	EXPECT_TRUE(EqualAbsError(90.0f, RadToDeg(kPi/2), 0.1f));
}


TEST(MTMath, IsNaN) {
	//number
	EXPECT_EQ(false, IsNaN(static_cast<int>(1)));
	EXPECT_EQ(false, IsNaN(static_cast<float>(1)));
	EXPECT_EQ(false, IsNaN(static_cast<double>(1)));
	EXPECT_EQ(false, IsNaN(static_cast<char>(1)));
}

TEST(MTMath, IsInf) {
	//number
	EXPECT_EQ(false, IsInf(static_cast<int>(1)));
	EXPECT_EQ(false, IsInf(static_cast<float>(1)));
	EXPECT_EQ(false, IsInf(static_cast<double>(1)));
	EXPECT_EQ(false, IsInf(static_cast<char>(1)));

	//inf
	float divider = 0.0f;
	float nan = 1.0f / divider;
	EXPECT_EQ(true, IsInf(nan));
}


TEST(MTMath, IsPower) {
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

TEST(MTMath, CeilPower) {
	//int test
	EXPECT_EQ(4, CeilPower(2, 4));
	EXPECT_EQ(8, CeilPower(2, 5));
	//double
	EXPECT_EQ(4.0, CeilPower(2.0, 4.0));
	EXPECT_EQ(9.0, CeilPower(3.0, 9.0));
	EXPECT_EQ(27.0, CeilPower(3.0, 9.1));
}
