#include "sora_test_stdafx.h"
#include "sora/coordinate.h"

using namespace std;
using namespace sora;

TEST(SphericalPoint, construct) {
	//일반적인 좌표
	SphericalPoint<float> p1 = SphericalPoint<float>::CreateDegreeBase(1.0f, 2.f, 3.f);
	EXPECT_EQ(1.0f, p1.radius());
	EXPECT_EQ(2.0f, p1.theta_deg());
	EXPECT_EQ(3.0f, p1.pi_deg());
	
	//radius가 음수인 경우 양수로 보정
	SphericalPoint<float> p2 = SphericalPoint<float>::CreateDegreeBase(-1.f, 2.f, 3.f);
	EXPECT_EQ(1.0f, p2.radius());
	
	//thera는 0~180사이로 유지
	SphericalPoint<float> p3 = SphericalPoint<float>::CreateDegreeBase(1.f, 190.f, 3.f);
	EXPECT_EQ(10.f, p3.theta_deg());
	
	p3 = SphericalPoint<float>::CreateDegreeBase(1.f, 370.f, 3.f);
	EXPECT_EQ(10.f, p3.theta_deg());
	
	p3 = SphericalPoint<float>::CreateDegreeBase(1.f, -10.f, 3.f);
	EXPECT_EQ(170.f, p3.theta_deg());
	
	//pi는 0~360사이로 유지
	SphericalPoint<float> p4 = SphericalPoint<float>::CreateDegreeBase(1.f, 100.f, 370.f);
	EXPECT_EQ(10.f, p4.pi_deg());
	
	p4 = SphericalPoint<float>::CreateDegreeBase(1.f, 100.f, 730.f);
	EXPECT_EQ(10.f, p4.pi_deg());
	
	p4 = SphericalPoint<float>::CreateDegreeBase(1.f, 100.f, -10.f);
	EXPECT_EQ(1.f, p4.radius());
	EXPECT_EQ(350.f, p4.pi_deg());
}

TEST(SphericalPoint, create) {
	//축위에 있을때의 변환
	//rectangular3 -> spherical
	//(1, 0, 0) -> (1, 90deg, 0deg);
	Rectangular3Point<float> orig(1, 0, 0);
	SphericalPoint<float> conv = SphericalPoint<float>::Create(orig);
	EXPECT_EQ(1.0f, conv.radius());
	EXPECT_TRUE(EqualAbsError(90.0f, conv.theta_deg(), 1.0f));
	EXPECT_TRUE(EqualAbsError(0.0f, conv.pi_deg(), 1.0f));
	
	//(0, 2, 0) -> (2, 90deg, 90deg);
	orig = Rectangular3Point<float>(0, 2, 0);
	conv = SphericalPoint<float>::Create(orig);
	EXPECT_EQ(2.0f, conv.radius());
	EXPECT_TRUE(EqualAbsError(90.0f, conv.theta_deg(), 1.0f));
	EXPECT_TRUE(EqualAbsError(90.0f, conv.pi_deg(), 1.0f));
	
	//(0, 0, 1) -> (1, 0deg, 0deg);
	orig = Rectangular3Point<float>(0, 0, 1);
	conv = SphericalPoint<float>::Create(orig);
	EXPECT_EQ(1.0f, conv.radius());
	EXPECT_TRUE(EqualAbsError(0.0f, conv.theta_deg(), 1.0f));
	EXPECT_TRUE(EqualAbsError(0.0f, conv.pi_deg(), 1.0f));
	
	//radius > 0
	orig = Rectangular3Point<float>(0.161136568, 0.247228201, -0.944041371);
	conv = SphericalPoint<float>::Create(orig);
	EXPECT_TRUE(conv.radius() > 0);

  {
    //01-26 00:21:56.859: I/Shared(11213): orig 0.064531 8.228765 5.586045
    //01-26 00:21:56.859: I/Shared(11213): 17.437447 89.550705->72.113258
    orig = Rectangular3Point<float>(+0.1, 1, 0);
    conv = SphericalPoint<float>::Create(orig);
    printf("%f / %f / %f\n", conv.radius(), conv.theta_deg(), conv.pi_deg());

    //01-26 00:21:56.879: I/Shared(11213): orig -0.270156 8.314498 5.621884
    //01-26 00:21:56.879: I/Shared(11213): 17.437447 271.860992->-105.576508
    //문제가 생기는건 pi
    //고작 x가 -붙었다고 각도가 확 바뀌는건 구현이 뭔가 잘못된거 일듯?
    //orig = Rectangular3Point<float>(-0.270156, 8.314498, 5.621884);
    orig = Rectangular3Point<float>(-0.1, 1, 0);
    conv = SphericalPoint<float>::Create(orig);
    printf("%f / %f / %f\n", conv.radius(), conv.theta_deg(), conv.pi_deg());
  }
  {
    orig = Rectangular3Point<float>(+0.1, -1, 0);
    conv = SphericalPoint<float>::Create(orig);
    printf("%f / %f / %f\n", conv.radius(), conv.theta_deg(), conv.pi_deg());

    orig = Rectangular3Point<float>(-0.1, -1, 0);
    conv = SphericalPoint<float>::Create(orig);
    printf("%f / %f / %f\n", conv.radius(), conv.theta_deg(), conv.pi_deg());
  }
}

TEST(Rectangular3Point, construct) {
	Rectangular3Point<float> p(1, 2, 3);
	EXPECT_EQ(1.0f, p.x());
	EXPECT_EQ(2.0f, p.y());
	EXPECT_EQ(3.0f, p.z());
}

