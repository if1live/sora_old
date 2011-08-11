#include "../SoraTestStdAfx.h"

#include "lynette/LNModel.h"

using namespace lynette;

TEST(LNModel, getStrideAndIsUseXX)
{
	//pos
	Model m1(LYNETTE_TRIANGLES);
	EXPECT_FALSE(m1.isUseColor());
	EXPECT_FALSE(m1.isUseTexCoord());
	EXPECT_FALSE(m1.isUseNormal());
	EXPECT_TRUE(3*sizeof(float) == m1.getStride());

	//pos + color
	Model m2(LYNETTE_TRIANGLES, LYNETTE_USE_COLOR);
	EXPECT_TRUE(m2.isUseColor());
	EXPECT_FALSE(m2.isUseTexCoord());
	EXPECT_FALSE(m2.isUseNormal());
	EXPECT_EQ(7*sizeof(float), (unsigned int)m2.getStride());
	
	//pos + tex + normal
	Model m3(LYNETTE_TRIANGLES, LYNETTE_USE_TEXCOORD | LYNETTE_USE_NORMAL);
	EXPECT_FALSE(m3.isUseColor());
	EXPECT_TRUE(m3.isUseTexCoord());
	EXPECT_TRUE(m3.isUseNormal());
	EXPECT_EQ(8*sizeof(float), (unsigned int)m3.getStride());
	
	//pos + tex
	Model m4(LYNETTE_LINES, LYNETTE_USE_TEXCOORD);
	EXPECT_FALSE(m4.isUseColor());
	EXPECT_TRUE(m4.isUseTexCoord());
	EXPECT_FALSE(m4.isUseNormal());
	EXPECT_EQ(5*sizeof(float), (unsigned int)m4.getStride());
	
}
