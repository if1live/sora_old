#include "../SoraTestStdAfx.h"
#include "chizuru/CHAccelerometerFilter.h"
#include "chizuru/CHInputEvent.h"
#include "matsu/MTMath.h"

using namespace chizuru;
using namespace matsu;

TEST(LowpassFilter, addAccelerometer)
{
	float x = -0.0362243652f;
	float y = -0.0362243652f;
	float z = -0.996170044f;
	vec4 data(x, y, z);
	
	LowpassFilter filter(100 - 80 + 1);
	filter.addAcceleration(data, 100);
	
	EXPECT_TRUE(isNormalNumber(filter.x));
	EXPECT_TRUE(isNormalNumber(filter.y));
	EXPECT_TRUE(isNormalNumber(filter.z));
}