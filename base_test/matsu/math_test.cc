// Ŭnicode please
#include "precompile.h"
#include "matsu/math_helper.h"

#include "matsu/vector.h"

using namespace std;
using namespace matsu;

TEST(Math, IsPower)
{
	//int test
	EXPECT_EQ(true, MathHelper::IsPower(2, 4));
	EXPECT_EQ(false, MathHelper::IsPower(2, 5));
	//double
	EXPECT_EQ(true, MathHelper::IsPower(2.0, 4.0));
	EXPECT_EQ(true, MathHelper::IsPower(3.0, 9.0));
	EXPECT_EQ(false, MathHelper::IsPower(3.0, 9.1));
	//float
	EXPECT_EQ(true, MathHelper::IsPower(2.0f, 4.0f));
	EXPECT_EQ(true, MathHelper::IsPower(3.0f, 9.0f));
	EXPECT_EQ(false, MathHelper::IsPower(3.0f, 9.1f));
}

TEST(Math, CeilPower)
{
	//int test
	EXPECT_EQ(4, MathHelper::CeilPower(2, 4));
	EXPECT_EQ(8, MathHelper::CeilPower(2, 5));
	//double
	EXPECT_EQ(4.0, MathHelper::CeilPower(2.0, 4.0));
	EXPECT_EQ(9.0, MathHelper::CeilPower(3.0, 9.0));
	EXPECT_EQ(27.0, MathHelper::CeilPower(3.0, 9.1));
}

TEST(Math, IsNaN)
{
	//number
	EXPECT_EQ(false, MathHelper::IsNaN(static_cast<int>(1)));
	EXPECT_EQ(false, MathHelper::IsNaN(static_cast<float>(1)));
	EXPECT_EQ(false, MathHelper::IsNaN(static_cast<double>(1)));
	EXPECT_EQ(false, MathHelper::IsNaN(static_cast<char>(1)));
}

TEST(Math, IsINF)
{
	//number
	EXPECT_EQ(false, MathHelper::IsINF(static_cast<int>(1)));
	EXPECT_EQ(false, MathHelper::IsINF(static_cast<float>(1)));
	EXPECT_EQ(false, MathHelper::IsINF(static_cast<double>(1)));
	EXPECT_EQ(false, MathHelper::IsINF(static_cast<char>(1)));

	//inf
	float divider = 0;
	float nan = 1.0f / divider;
	EXPECT_EQ(true, MathHelper::IsINF(nan));
}

TEST(Math, IsNormalNumber)
{
	EXPECT_EQ(true, MathHelper::IsNormalNumber(static_cast<int>(1)));
	EXPECT_EQ(true, MathHelper::IsNormalNumber(static_cast<float>(1)));
	
	//inf
	float divider = 0;
	float inf = 1.0f / divider;
	EXPECT_EQ(false, MathHelper::IsNormalNumber(inf));
}

TEST(Math, StringToInt)
{
	string str = "123";
	EXPECT_EQ(123, MathHelper::StringToInt(str));

	str = "-123";
	EXPECT_EQ(-123, MathHelper::StringToInt(str));

	str = "0";
	EXPECT_EQ(0, MathHelper::StringToInt(str));
	
	//not valid number
	str = "fdfds";
	EXPECT_EQ(0, MathHelper::StringToInt(str));
}

TEST(Math, StringToFloat)
{
	string str = "123.23";
	EXPECT_EQ(123.23f, MathHelper::StringToFloat(str));

	str = "123.23";
	EXPECT_EQ(123.23, MathHelper::StringToFloat(str));

	str = "0";
	EXPECT_EQ(0, MathHelper::StringToFloat(str));
	
	//not valid number
	str = "fdfds";
	EXPECT_EQ(0, MathHelper::StringToFloat(str));
}

TEST(Math, CalculateIncludeAngleRadian)
{
	float radian1 = 0.0067581712f;
	float radian2 = 0.0066478122f;
	MathHelper::CalculateIncludeAngleRadian(radian1, radian2);
}

TEST(Math, Lerp)
{
	//float
	float f1 = 10;
	float f2 = 100;
	EXPECT_EQ(10, MathHelper::Lerp(f1, f2, 0));
	EXPECT_EQ(100, MathHelper::Lerp(f1, f2, 1));
	EXPECT_EQ(55, MathHelper::Lerp(f1, f2, 0.5));
	
	//vector
	vec2 v1(1, 1);
	vec2 v2(100, 20);
	EXPECT_TRUE(v1 == MathHelper::Lerp(v1, v2, 0));
	EXPECT_TRUE(v2 == MathHelper::Lerp(v1, v2, 1));
	EXPECT_TRUE(vec2((1+100)/2.0, (1+20)/2.0) == MathHelper::Lerp(v1, v2, 0.5));
}