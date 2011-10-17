#include "precompile.h"
#include "matsu/angle.h"
#include "matsu/math.h"

using namespace matsu;

TEST(Angle, test)
{
  Angle<float> a1 = Angle<float>::Degree(180);
  Angle<float> a2 = Angle<float>::Radian(kPi);
  EXPECT_EQ(true, a1 == a2);

  Angle<float> a3 = Angle<float>::Degree(360 + 180);
  EXPECT_EQ(true, a1 == a3);

  Angle<float> a4 = Angle<float>::Degree(360 * 2 + 180);
  EXPECT_EQ(true, a1 == a4);
}

TEST(Angle, DegreeToRadian)
{
	//180degree = pi rad
	EXPECT_TRUE(EqualAbsoluteError(kPi, Angle<float>::DegreeToRadian(180.0f), 0.1f));
	//90degree = 1/2 pi rad
	EXPECT_TRUE(EqualAbsoluteError(kPi/2, Angle<float>::DegreeToRadian(90.0f), 0.1f));
}

TEST(Angle, RadianToDegree)
{
	//pi rad = 180degree
	EXPECT_TRUE(EqualAbsoluteError(180.0f, Angle<float>::RadianToDegree(kPi), 0.1f));
	//1/2pi rad = 90degree
	EXPECT_TRUE(EqualAbsoluteError(90.0f, Angle<float>::RadianToDegree(kPi/2), 0.1f));
}
