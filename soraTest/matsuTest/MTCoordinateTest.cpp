#include "../SoraTestStdAfx.h"
#include "matsu/MTCoordinate.h"

using namespace std;
using namespace matsu;

TEST(SphericalPoint, construct)
{
	//일반적인 좌표
	SphericalPoint<float> p1 = SphericalPoint<float>::createDegreeBase(1.0f, 2.f, 3.f);
	EXPECT_EQ(1.0f, p1.getRadius());
	EXPECT_EQ(2.0f, p1.getThetaDegree());
	EXPECT_EQ(3.0f, p1.getPiDegree());
	
	//radius가 음수인 경우 양수로 보정
	SphericalPoint<float> p2 = SphericalPoint<float>::createDegreeBase(-1.f, 2.f, 3.f);
	EXPECT_EQ(1.0f, p2.getRadius());
	
	//thera는 0~180사이로 유지
	SphericalPoint<float> p3 = SphericalPoint<float>::createDegreeBase(1.f, 190.f, 3.f);
	EXPECT_EQ(10.f, p3.getThetaDegree());
	
	p3 = SphericalPoint<float>::createDegreeBase(1.f, 370.f, 3.f);
	EXPECT_EQ(10.f, p3.getThetaDegree());
	
	p3 = SphericalPoint<float>::createDegreeBase(1.f, -10.f, 3.f);
	EXPECT_EQ(170.f, p3.getThetaDegree());
	
	//pi는 0~360사이로 유지
	SphericalPoint<float> p4 = SphericalPoint<float>::createDegreeBase(1.f, 100.f, 370.f);
	EXPECT_EQ(10.f, p4.getPiDegree());
	
	p4 = SphericalPoint<float>::createDegreeBase(1.f, 100.f, 730.f);
	EXPECT_EQ(10.f, p4.getPiDegree());
	
	p4 = SphericalPoint<float>::createDegreeBase(1.f, 100.f, -10.f);
	EXPECT_EQ(1.f, p4.getRadius());
	EXPECT_EQ(350.f, p4.getPiDegree());
}

TEST(SphericalPoint, create)
{
	//축위에 있을때의 변환
	//rectangular3 -> spherical
	//(1, 0, 0) -> (1, 90deg, 0deg);
	Rectangular3Point<float> orig(1, 0, 0);
	SphericalPoint<float> conv = SphericalPoint<float>::create(orig);
	EXPECT_EQ(1.0f, conv.getRadius());
	EXPECT_TRUE(equalAbsError(90.0f, conv.getThetaDegree(), 1.0f));
	EXPECT_TRUE(equalAbsError(0.0f, conv.getPiDegree(), 1.0f));
	
	//(0, 2, 0) -> (2, 90deg, 90deg);
	orig = Rectangular3Point<float>(0, 2, 0);
	conv = SphericalPoint<float>::create(orig);
	EXPECT_EQ(2.0f, conv.getRadius());
	EXPECT_TRUE(equalAbsError(90.0f, conv.getThetaDegree(), 1.0f));
	EXPECT_TRUE(equalAbsError(90.0f, conv.getPiDegree(), 1.0f));
	
	//(0, 0, 1) -> (1, 0deg, 0deg);
	orig = Rectangular3Point<float>(0, 0, 1);
	conv = SphericalPoint<float>::create(orig);
	EXPECT_EQ(1.0f, conv.getRadius());
	EXPECT_TRUE(equalAbsError(0.0f, conv.getThetaDegree(), 1.0f));
	EXPECT_TRUE(equalAbsError(0.0f, conv.getPiDegree(), 1.0f));
	
	//radius > 0
	orig = Rectangular3Point<float>(0.161136568, 0.247228201, -0.944041371);
	conv = SphericalPoint<float>::create(orig);
	EXPECT_TRUE(conv.getRadius() > 0);
}

TEST(Rectangular3Point, construct)
{
	Rectangular3Point<float> p(1, 2, 3);
	EXPECT_EQ(1.0f, p.getX());
	EXPECT_EQ(2.0f, p.getY());
	EXPECT_EQ(3.0f, p.getZ());
}

