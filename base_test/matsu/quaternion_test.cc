#include "precompile.h"

#include "matsu/quaternion.h"

using namespace std;
using namespace matsu;

TEST(Quaternion, Conjugate)
{
	quat q1(1, 2, 3, 4);
	quat q2 = q1.Conjugation();
	EXPECT_EQ(-1, q2.x);
	EXPECT_EQ(-2, q2.y);
	EXPECT_EQ(-3, q2.z);
	EXPECT_EQ(4, q2.w);
}
