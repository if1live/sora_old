// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "matsu/MTMath.h"

#include "matsu/MTVector.h"

using namespace std;
using namespace matsu;

TEST(MTMath, degree2radian)
{
	//180degree = pi rad
	EXPECT_TRUE(equalAbsError(MT_PI, degree2radian(180.0f), 0.1f));
	//90degree = 1/2 pi rad
	EXPECT_TRUE(equalAbsError(MT_PI/2, degree2radian(90.0f), 0.1f));
}

TEST(MTMath, radian2degree)
{
	//pi rad = 180degree
	EXPECT_TRUE(equalAbsError(180.0f, radian2degree(MT_PI), 0.1f));
	//1/2pi rad = 90degree
	EXPECT_TRUE(equalAbsError(90.0f, radian2degree(MT_PI/2), 0.1f));
}

TEST(MTMath, isPower)
{
	//int test
	EXPECT_EQ(true, isPower(2, 4));
	EXPECT_EQ(false, isPower(2, 5));
	//double
	EXPECT_EQ(true, isPower(2.0, 4.0));
	EXPECT_EQ(true, isPower(3.0, 9.0));
	EXPECT_EQ(false, isPower(3.0, 9.1));
	//float
	EXPECT_EQ(true, isPower(2.0f, 4.0f));
	EXPECT_EQ(true, isPower(3.0f, 9.0f));
	EXPECT_EQ(false, isPower(3.0f, 9.1f));
}

TEST(MTMath, ceilPower)
{
	//int test
	EXPECT_EQ(4, ceilPower(2, 4));
	EXPECT_EQ(8, ceilPower(2, 5));
	//double
	EXPECT_EQ(4.0, ceilPower(2.0, 4.0));
	EXPECT_EQ(9.0, ceilPower(3.0, 9.0));
	EXPECT_EQ(27.0, ceilPower(3.0, 9.1));
}

TEST(MTMath, isNaN)
{
	//number
	EXPECT_EQ(false, isNaN(static_cast<int>(1)));
	EXPECT_EQ(false, isNaN(static_cast<float>(1)));
	EXPECT_EQ(false, isNaN(static_cast<double>(1)));
	EXPECT_EQ(false, isNaN(static_cast<char>(1)));
}

TEST(MTMath, isInf)
{
	//number
	EXPECT_EQ(false, isInf(static_cast<int>(1)));
	EXPECT_EQ(false, isInf(static_cast<float>(1)));
	EXPECT_EQ(false, isInf(static_cast<double>(1)));
	EXPECT_EQ(false, isInf(static_cast<char>(1)));

	//inf
	float divider = 0;
	float nan = 1.0 / divider;
	EXPECT_EQ(true, isInf(nan));
}

TEST(MTMath, isNormalNumber)
{
	EXPECT_EQ(true, isNormalNumber(static_cast<int>(1)));
	EXPECT_EQ(true, isNormalNumber(static_cast<float>(1)));
	
	//inf
	float divider = 0;
	float inf = 1.0 / divider;
	EXPECT_EQ(false, isNormalNumber(inf));
}

TEST(MTMath, str2int)
{
	string str = "123";
	EXPECT_EQ(123, str2int<int>(str));

	str = "-123";
	EXPECT_EQ(-123, str2int<int>(str));

	str = "0";
	EXPECT_EQ(0, str2int<int>(str));
	
	//not valid number
	str = "fdfds";
	EXPECT_EQ(0, str2int<int>(str));
}

TEST(MTMath, str2float)
{
	string str = "123.23";
	EXPECT_EQ(123.23f, str2float<float>(str));

	str = "123.23";
	EXPECT_EQ(123.23, str2float<double>(str));

	str = "0";
	EXPECT_EQ(0, str2float<float>(str));
	
	//not valid number
	str = "fdfds";
	EXPECT_EQ(0, str2float<float>(str));
}

TEST(MTMath, calcIncludeAngleRadian)
{
	float radian1 = 0.0067581712f;
	float radian2 = 0.0066478122f;
	calcIncludeAngleRadian(radian1, radian2);
}

TEST(MTMath, lerp)
{
	//float
	float f1 = 10;
	float f2 = 100;
	EXPECT_EQ(10, lerp(f1, f2, 0));
	EXPECT_EQ(100, lerp(f1, f2, 1));
	EXPECT_EQ(55, lerp(f1, f2, 0.5));
	
	//vector
	vec2 v1(1, 1);
	vec2 v2(100, 20);
	EXPECT_TRUE(v1 == lerp(v1, v2, 0));
	EXPECT_TRUE(v2 == lerp(v1, v2, 1));
	EXPECT_TRUE(vec2((1+100)/2.0, (1+20)/2.0) == lerp(v1, v2, 0.5));
}