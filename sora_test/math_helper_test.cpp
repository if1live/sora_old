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
	float divider = 0.0f;
	float nan = 1.0f / divider;
	EXPECT_EQ(true, IsInf(nan));
}
