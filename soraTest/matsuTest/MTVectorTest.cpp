// Ŭnicode please
#include "../SoraTestStdAfx.h"

#include "matsu/MTVector.h"

using namespace matsu;

TEST(VectorTpl, get_set)
{
	VectorTpl<int, 2> v1;
	//기본값
	EXPECT_EQ(0, v1.get(0));
	EXPECT_EQ(0, v1.get(1));
	EXPECT_EQ(0, v1.get<0>());
	EXPECT_EQ(0, v1.get<1>());
	
	v1.set(0, 10);
	v1.set<1>(20);
	EXPECT_EQ(10, v1.get(0));
	EXPECT_EQ(20, v1.get(1));
	EXPECT_EQ(10, v1.get<0>());
	EXPECT_EQ(20, v1.get<1>());
	
	//배열로 설정하기
	int data[] = {1, 2};
	v1.set(data);
	EXPECT_EQ(1, v1.get<0>());
	EXPECT_EQ(2, v1.get<1>());
	
	//배열이 더 작은 경우
	int data2[] = {11};
	v1.set(data2);
	EXPECT_EQ(11, v1.get<0>());
	EXPECT_EQ(0, v1.get<1>());
	
	//배열이 더 큰 경우
	int data3[] = {3, 4, 5};
	v1.set(data3);
	EXPECT_EQ(3, v1.get<0>());
	EXPECT_EQ(4, v1.get<1>());
}

TEST(VectorTpl, construct)
{
	//그냥만들면 요소는 0
	VectorTpl<int,2> v1;
	EXPECT_EQ(0, v1.get<0>());
	EXPECT_EQ(0, v1.get<1>());
	
	//배열로 만들기
	int data1[] = {1, 2};
	VectorTpl<int,2> v2(data1);
	EXPECT_EQ(1, v2.get<0>());
	EXPECT_EQ(2, v2.get<1>());
	
	//복사생성자 
	VectorTpl<int,2> v3(v2);
	EXPECT_EQ(1, v3.get<0>());
	EXPECT_EQ(2, v3.get<1>());
	
	//타입 교체
	VectorTpl<float,2> v4(v3);
	EXPECT_EQ(1.0f, v4.get<0>());
	EXPECT_EQ(2.0f, v4.get<1>());
	
	//값 있는거 넣어서 초기화
	VectorTpl<int,3> v5(1, 2);
	EXPECT_EQ(1, v5.get<0>());
	EXPECT_EQ(2, v5.get<1>());
	EXPECT_EQ(0, v5.get<2>());
	
	VectorTpl<int,3> v6(1, 2, 3);
	EXPECT_EQ(1, v6.get<0>());
	EXPECT_EQ(2, v6.get<1>());
	EXPECT_EQ(3, v6.get<2>());
	
	VectorTpl<int,3> v7(1, 2, 3, 4);
	EXPECT_EQ(1, v7.get<0>());
	EXPECT_EQ(2, v7.get<1>());
	EXPECT_EQ(3, v7.get<2>());
	
	//높은 차원의 벡터로 초기화
	VectorTpl<int,4> v8(1, 2, 3, 4);
	VectorTpl<int,3> v9(v8);
	EXPECT_EQ(1, v9.get<0>());
	EXPECT_EQ(2, v9.get<1>());
	EXPECT_EQ(3, v9.get<2>());
	
	//낮은 차원의 벡터로 초기화
	VectorTpl<int,2> v10(1, 2);
	VectorTpl<int,3> v11(v10);
	EXPECT_EQ(1, v11.get<0>());
	EXPECT_EQ(2, v11.get<1>());
	EXPECT_EQ(0, v11.get<2>());
}

TEST(VectorTpl, addMinus)
{
	int data1[] = {1, 2};
	VectorTpl<int,2> v1(data1);
	
	int data2[] = {3, 4};
	VectorTpl<int,2> v2(data2);
	
	//test +=
	v2 += v1;
	EXPECT_EQ(4, v2.get<0>());
	EXPECT_EQ(6, v2.get<1>());
	
	VectorTpl<int,2> v3 = v1 + v2;
	EXPECT_EQ(5, v3.get<0>());
	EXPECT_EQ(8, v3.get<1>());
	
	//인자없는 -
	v3 = -v3;
	EXPECT_EQ(-5, v3.get<0>());
	EXPECT_EQ(-8, v3.get<1>());
}

TEST(VectorTpl, xyzw)
{
	int data2[] = {3, 4};
	VectorTpl<int,2> v2(data2);
	
	EXPECT_EQ(3, v2.x());
	EXPECT_EQ(4, v2.y());
	//EXPECT_EQ(0, v2.z());
	//EXPECT_EQ(0, v2.w());
	
	v2.setX(1);
	v2.setY(2);
	//v2.setZ(3);
	//v2.setW(4);
	
	EXPECT_EQ(1, v2.x());
	EXPECT_EQ(2, v2.y());
	//EXPECT_EQ(0, v2.z());
	//EXPECT_EQ(0, v2.w());
}

TEST(VectorTpl, size)
{
	int data1[] = {3, 4};
	VectorTpl<int,2> v1(data1);
	EXPECT_EQ(25, v1.sizeSquare());
	EXPECT_EQ(5, v1.size());
}

TEST(VectorTpl, dot)
{
	int data1[] = {1, 2};
	VectorTpl<int,2> v1(data1);
	int data2[] = {3, 4};
	VectorTpl<int,2> v2(data2);
	
	EXPECT_EQ(11, v1.dot(v2));
	EXPECT_EQ(11, v2.dot(v1));
}

TEST(VectorTpl, eq)
{
	int data1[] = {1, 2};
	VectorTpl<int,2> v1(data1);
	int data2[] = {3, 4};
	VectorTpl<int,2> v2(data2);
	VectorTpl<int,2> v3(data2);
	EXPECT_TRUE(v1 != v2);
	EXPECT_TRUE(v3 == v2);
}

TEST(VectorTpl, dimension)
{
	VectorTpl<int,2> v1;
	VectorTpl<float,3> v2;
	int dim = VectorTpl<int,2>::dimension;
	EXPECT_EQ(2, dim);
	
	EXPECT_EQ(2, v1.getDim());
	EXPECT_EQ(3, v2.getDim());
}

TEST(VectorTpl, pointer)
{
	VectorTpl<int,2> v1(1, 2);
	int *ptr = v1.pointer();
	EXPECT_EQ(1, ptr[0]);
	EXPECT_EQ(2, ptr[1]);
}

TEST(VectorTpl, isValidIndex)
{
	VectorTpl<int,2> v1;
	EXPECT_EQ(true, v1.isValidIndex(0));
	EXPECT_EQ(true, v1.isValidIndex(1));
	EXPECT_EQ(false, v1.isValidIndex(2));
	
	VectorTpl<int,3> v2;
	EXPECT_EQ(true, v2.isValidIndex(0));
	EXPECT_EQ(true, v2.isValidIndex(1));
	EXPECT_EQ(true, v2.isValidIndex(2));
	EXPECT_EQ(false, v2.isValidIndex(3));
}

TEST(VectorTpl, cross)
{
	VectorTpl<int,3> xVec(1, 0, 0);
	VectorTpl<int,3> yVec(0, 1, 0);
	VectorTpl<int,3> result = xVec.cross(yVec);
	EXPECT_EQ(0, result.get<0>());
	EXPECT_EQ(0, result.get<1>());
	EXPECT_EQ(1, result.get<2>());
}