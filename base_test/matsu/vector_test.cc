// Ŭnicode please
#include "precompile.h"
#include "matsu/vector.h"

using namespace matsu;

TEST(VectorTemplate, construct)
{
	//그냥만들면 요소는 0
	VectorTemplate<int,2> v1;
	EXPECT_EQ(0, v1.data[0]);
	EXPECT_EQ(0, v1.data[1]);
	
	//배열로 만들기
	int data1[] = {1, 2};
	VectorTemplate<int,2> v2(data1);
	EXPECT_EQ(1, v2.data[0]);
	EXPECT_EQ(2, v2.data[1]);
	
	//복사생성자 
	VectorTemplate<int,2> v3(v2);
	EXPECT_EQ(1, v3.data[0]);
	EXPECT_EQ(2, v3.data[1]);
	
	//타입 교체
	VectorTemplate<float,2> v4(v3);
	EXPECT_EQ(1.0f, v4.data[0]);
	EXPECT_EQ(2.0f, v4.data[1]);
	
	//값 있는거 넣어서 초기화
	VectorTemplate<int,3> v5(1, 2);
	EXPECT_EQ(1, v5.data[0]);
	EXPECT_EQ(2, v5.data[1]);
	EXPECT_EQ(0, v5.data[2]);
	
	VectorTemplate<int,3> v6(1, 2, 3);
	EXPECT_EQ(1, v6.data[0]);
	EXPECT_EQ(2, v6.data[1]);
	EXPECT_EQ(3, v6.data[2]);
	
	VectorTemplate<int,3> v7(1, 2, 3, 4);
	EXPECT_EQ(1, v7.data[0]);
	EXPECT_EQ(2, v7.data[1]);
	EXPECT_EQ(3, v7.data[2]);
	
	//높은 차원의 벡터로 초기화
	VectorTemplate<int,4> v8(1, 2, 3, 4);
	VectorTemplate<int,3> v9(v8);
	EXPECT_EQ(1, v9.data[0]);
	EXPECT_EQ(2, v9.data[1]);
	EXPECT_EQ(3, v9.data[2]);
	
	//낮은 차원의 벡터로 초기화
	VectorTemplate<int,2> v10(1, 2);
	VectorTemplate<int,3> v11(v10);
	EXPECT_EQ(1, v11.data[0]);
	EXPECT_EQ(2, v11.data[1]);
	EXPECT_EQ(0, v11.data[2]);
}

TEST(VectorTemplate, addMinus)
{
	int data1[] = {1, 2};
	VectorTemplate<int,2> v1(data1);
	
	int data2[] = {3, 4};
	VectorTemplate<int,2> v2(data2);
	
	//test +=
	v2 += v1;
	EXPECT_EQ(4, v2.data[0]);
	EXPECT_EQ(6, v2.data[1]);
	
	VectorTemplate<int,2> v3 = v1 + v2;
	EXPECT_EQ(5, v3.data[0]);
	EXPECT_EQ(8, v3.data[1]);
	
	//인자없는 -
	v3 = -v3;
	EXPECT_EQ(-5, v3.data[0]);
	EXPECT_EQ(-8, v3.data[1]);
}

TEST(VectorTemplate, xyzw)
{
	int data2[] = {3, 4};
	VectorTemplate<int,2> v2(data2);
	
	EXPECT_EQ(3, v2.x());
	EXPECT_EQ(4, v2.y());
	//EXPECT_EQ(0, v2.z());
	//EXPECT_EQ(0, v2.w());
	
	v2.set_x(1);
	v2.set_y(2);
	//v2.setZ(3);
	//v2.setW(4);
	
	EXPECT_EQ(1, v2.x());
	EXPECT_EQ(2, v2.y());
	//EXPECT_EQ(0, v2.z());
	//EXPECT_EQ(0, v2.w());
}

TEST(VectorTemplate, Size)
{
	int data1[] = {3, 4};
	VectorTemplate<int,2> v1(data1);
	EXPECT_EQ(25, v1.SizeSquare());
	EXPECT_EQ(5, v1.Size());
}

TEST(VectorTemplate, Dot)
{
	int data1[] = {1, 2};
	VectorTemplate<int,2> v1(data1);
	int data2[] = {3, 4};
	VectorTemplate<int,2> v2(data2);
	
	EXPECT_EQ(11, v1.Dot(v2));
	EXPECT_EQ(11, v2.Dot(v1));
}

TEST(VectorTemplate, eq)
{
	int data1[] = {1, 2};
	VectorTemplate<int,2> v1(data1);
	int data2[] = {3, 4};
	VectorTemplate<int,2> v2(data2);
	VectorTemplate<int,2> v3(data2);
	EXPECT_TRUE(v1 != v2);
	EXPECT_TRUE(v3 == v2);
}

TEST(VectorTemplate, Dimension)
{
	VectorTemplate<int,2> v1;
	VectorTemplate<float,3> v2;
	int dim = VectorTemplate<int,2>::Dimension;
	EXPECT_EQ(2, dim);
	
	EXPECT_EQ(2, v1.GetDimension());
	EXPECT_EQ(3, v2.GetDimension());
}

TEST(VectorTemplate, Pointer)
{
	VectorTemplate<int,2> v1(1, 2);
	int *ptr = v1.Pointer();
	EXPECT_EQ(1, ptr[0]);
	EXPECT_EQ(2, ptr[1]);
}

TEST(VectorTemplate, IsValidIndex)
{
	VectorTemplate<int,2> v1;
	EXPECT_EQ(true, v1.IsValidIndex(0));
	EXPECT_EQ(true, v1.IsValidIndex(1));
	EXPECT_EQ(false, v1.IsValidIndex(2));
	
	VectorTemplate<int,3> v2;
	EXPECT_EQ(true, v2.IsValidIndex(0));
	EXPECT_EQ(true, v2.IsValidIndex(1));
	EXPECT_EQ(true, v2.IsValidIndex(2));
	EXPECT_EQ(false, v2.IsValidIndex(3));
}

TEST(VectorTemplate, Cross)
{
	VectorTemplate<int,3> xVec(1, 0, 0);
	VectorTemplate<int,3> yVec(0, 1, 0);
	VectorTemplate<int,3> result = xVec.Cross(yVec);
	EXPECT_EQ(0, result.data[0]);
	EXPECT_EQ(0, result.data[1]);
	EXPECT_EQ(1, result.data[2]);
}