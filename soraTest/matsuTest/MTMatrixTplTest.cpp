#include "../SoraTestStdAfx.h"
#include "matsu/MTMatrixTpl.h"

using namespace matsu;

TEST(MTMatrixTpl, construct)
{
	//인자없이 생성하면 가급적 identity의 근접하게 한다
	MatrixTpl<int, 2, 2> m1;
	EXPECT_EQ(1, m1.get(0, 0));
	EXPECT_EQ(0, m1.get(0, 1));
	EXPECT_EQ(0, m1.get(1, 0));
	EXPECT_EQ(1, m1.get(1, 1));
	
	MatrixTpl<int, 3, 3> m2;
	EXPECT_EQ(1, m2.get(0, 0));
	EXPECT_EQ(0, m2.get(0, 1));
	EXPECT_EQ(0, m2.get(0, 2));
	
	EXPECT_EQ(0, m2.get(1, 0));
	EXPECT_EQ(1, m2.get(1, 1));
	EXPECT_EQ(0, m2.get(1, 2));
	
	EXPECT_EQ(0, m2.get(2, 0));
	EXPECT_EQ(0, m2.get(2, 1));
	EXPECT_EQ(1, m2.get(2, 2));
	
	//2*3의 경우, 3행은 0이다
	MatrixTpl<int,2,3> m3;
	EXPECT_EQ(1, m3.get(0, 0));
	EXPECT_EQ(0, m3.get(0, 1));
	EXPECT_EQ(0, m3.get(0, 2));
	
	EXPECT_EQ(0, m3.get(1, 0));
	EXPECT_EQ(1, m3.get(1, 1));
	EXPECT_EQ(0, m3.get(1, 2));
	
	//배열을 기반으로 생성하기
	int data1[] = {1, 0,	0, 1,	0, 0};
	MatrixTpl<int,2,3> m4(data1);
	EXPECT_EQ(1, m4.get(0, 0));
	EXPECT_EQ(0, m4.get(0, 1));
	EXPECT_EQ(0, m4.get(0, 2));
	
	EXPECT_EQ(0, m4.get(1, 0));
	EXPECT_EQ(1, m4.get(1, 1));
	EXPECT_EQ(0, m4.get(1, 2));
	
	//높은 차원을 낮은 차원으로
	//1 2 3
	//4 5 6
	//7 8 9
	int data2[] = {1,4,7,	2,5,8,	3,6,9};
	MatrixTpl<int,3,3> m5(data2);
	MatrixTpl<int,2,2> m6(m5);
	EXPECT_EQ(1, m6.get(0, 0));
	EXPECT_EQ(2, m6.get(0, 1));
	EXPECT_EQ(4, m6.get(1, 0));
	EXPECT_EQ(5, m6.get(1, 1));
}

TEST(MTMatrixTpl, addMinus)
{
	//TOOD
	
}

TEST(MTMatrixTpl, transpose)
{
	int data1[] = {1, 0,	0, 1,	3, 4};
	MatrixTpl<int,2,3> m1(data1);
	MatrixTpl<int,3,2> m2 = m1.transpose();
	EXPECT_EQ(1, m2.get(0, 0));
	EXPECT_EQ(0, m2.get(0, 1));
	
	EXPECT_EQ(0, m2.get(1, 0));
	EXPECT_EQ(1, m2.get(1, 1));
	
	EXPECT_EQ(3, m2.get(2, 0));
	EXPECT_EQ(4, m2.get(2, 1));
}

TEST(MTMaxtrixTpl, getRow)
{
	//1 0 3
	//0 1 4
	int data1[] = {1, 0,	0, 1,	3, 4};
	MatrixTpl<int,2,3> m1(data1);
	VectorTpl<int,3> v1 = m1.getRow(0);
	VectorTpl<int,3> v2 = m1.getRow(1);
	
	VectorTpl<int,3> t1(1, 0, 3);
	VectorTpl<int,3> t2(0, 1, 4);
	EXPECT_TRUE(v1 == t1);
	EXPECT_TRUE(v2 == t2);
}

TEST(MTMatrixTpl, getCol)
{
	//1 0 3
	//0 1 4
	int data1[] = {1, 0,	0, 1,	3, 4};
	MatrixTpl<int,2,3> m1(data1);
	VectorTpl<int,2> v1 = m1.getCol(0);
	VectorTpl<int,2> v2 = m1.getCol(1);
	VectorTpl<int,2> v3 = m1.getCol(2);
	
	VectorTpl<int,2> t1(1, 0);
	VectorTpl<int,2> t2(0, 1);
	VectorTpl<int,2> t3(3, 4);
	EXPECT_TRUE(v1 == t1);
	EXPECT_TRUE(v2 == t2);
	EXPECT_TRUE(v3 == t3);
}

TEST(MTMatrixTpl, multiple)
{
	//1 2 3
	//3 2 1
	int data1[] = {1, 3,	2, 2,	3, 1};
	//1 0
	//0 1
	//1 1
	int data2[] = {1, 0, 1,		0, 1, 1};
	MatrixTpl<int,2,3> m1(data1);
	MatrixTpl<int,3,2> m2(data2);
	MatrixTpl<int,2,2> m = m1 * m2;
	
	EXPECT_EQ(4, m.get(0, 0));
	EXPECT_EQ(5, m.get(0, 1));
	EXPECT_EQ(4, m.get(1, 0));
	EXPECT_EQ(3, m.get(1, 1));	
}