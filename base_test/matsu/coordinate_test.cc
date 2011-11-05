#include "precompile.h"
#include "matsu/coordinate.h"

using namespace std;
using namespace matsu;

TEST(SphericalPoint, construct)
{
	//일반적인 좌표
	SphericalPoint<float> p1 = SphericalPoint<float>::CreateDegreeBase(1.0f, 2.f, 3.f);
	EXPECT_EQ(1.0f, p1.radius());
	EXPECT_EQ(2.0f, p1.theta_degree());
	EXPECT_EQ(3.0f, p1.pi_degree());
	
	//radius가 음수인 경우 양수로 보정
	SphericalPoint<float> p2 = SphericalPoint<float>::CreateDegreeBase(-1.f, 2.f, 3.f);
	EXPECT_EQ(1.0f, p2.radius());
	
	//thera는 0~180사이로 유지
	SphericalPoint<float> p3 = SphericalPoint<float>::CreateDegreeBase(1.f, 190.f, 3.f);
	EXPECT_EQ(10.f, p3.theta_degree());
	
	p3 = SphericalPoint<float>::CreateDegreeBase(1.f, 370.f, 3.f);
	EXPECT_EQ(10.f, p3.theta_degree());
	
	p3 = SphericalPoint<float>::CreateDegreeBase(1.f, -10.f, 3.f);
	EXPECT_EQ(170.f, p3.theta_degree());
	
	//pi는 0~360사이로 유지
	SphericalPoint<float> p4 = SphericalPoint<float>::CreateDegreeBase(1.f, 100.f, 370.f);
	EXPECT_EQ(10.f, p4.pi_degree());
	
	p4 = SphericalPoint<float>::CreateDegreeBase(1.f, 100.f, 730.f);
	EXPECT_EQ(10.f, p4.pi_degree());
	
	p4 = SphericalPoint<float>::CreateDegreeBase(1.f, 100.f, -10.f);
	EXPECT_EQ(1.f, p4.radius());
  EXPECT_EQ(true, Angle<float>(350.f).GetDegree() == p4.pi_degree());
}

TEST(SphericalPoint, Create)
{
	//축위에 있을때의 변환
	//rectangular3 -> spherical
	//(1, 0, 0) -> (1, 90deg, 0deg);
	Rectangular3Point<float> orig(1, 0, 0);
	SphericalPoint<float> conv = SphericalPoint<float>::Create(orig);
	EXPECT_EQ(1.0f, conv.radius());
	EXPECT_TRUE(MathHelper::EqualAbsoluteError(90.0f, conv.theta_degree(), 1.0f));
	EXPECT_TRUE(MathHelper::EqualAbsoluteError(0.0f, conv.pi_degree(), 1.0f));
	
	//(0, 2, 0) -> (2, 90deg, 90deg);
	orig = Rectangular3Point<float>(0, 2, 0);
	conv = SphericalPoint<float>::Create(orig);
	EXPECT_EQ(2.0f, conv.radius());
	EXPECT_TRUE(MathHelper::EqualAbsoluteError(90.0f, conv.theta_degree(), 1.0f));
	EXPECT_TRUE(MathHelper::EqualAbsoluteError(90.0f, conv.pi_degree(), 1.0f));
	
	//(0, 0, 1) -> (1, 0deg, 0deg);
	orig = Rectangular3Point<float>(0, 0, 1);
	conv = SphericalPoint<float>::Create(orig);
	EXPECT_EQ(1.0f, conv.radius());
	EXPECT_TRUE(MathHelper::EqualAbsoluteError(0.0f, conv.theta_degree(), 1.0f));
	EXPECT_TRUE(MathHelper::EqualAbsoluteError(0.0f, conv.pi_degree(), 1.0f));
	
	//radius > 0
	orig = Rectangular3Point<float>(0.161136568f, 0.247228201f, -0.944041371f);
	conv = SphericalPoint<float>::Create(orig);
	EXPECT_TRUE(conv.radius() > 0);
}
