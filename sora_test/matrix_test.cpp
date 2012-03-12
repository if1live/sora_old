/*  Copyright (C) 2011-2012 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "sora_test_stdafx.h"
#include "sora/matrix.h"

//2dim -> 1dim
//int (&c)[2][2] = (int(&)[2][2])a;

TEST(Matrix, ArrayCast) {
  using namespace sora;
  {
    int a[2][3] = {{0, 1, 2}, {3, 4, 5}};
	  auto b = flatten(a);
	  print(flatten(a));
  }
  {
    int a[4] = {0, 1, 2, 3};
    auto b = twodim(a);
    print(twodim(a));
  }
}

TEST(Dim, dim) {
  using namespace sora;
  EXPECT_EQ(2, (Dim<4>::value));
  EXPECT_EQ(3, (Dim<9>::value));
  EXPECT_EQ(4, (Dim<16>::value));
}

TEST(Matrix, MatSetIdentity) {
  using namespace sora;
  Mat2f m1;
  MatSetIdentity(m1);
  EXPECT_EQ(1, (m1[GetIndex<2, 0, 0>()]));
  EXPECT_EQ(0, (m1[GetIndex<2, 0, 1>()]));
  EXPECT_EQ(0, (m1[GetIndex<2, 1, 0>()]));
  EXPECT_EQ(1, (m1[GetIndex<2, 1, 1>()]));
}


TEST(MathHelper, MatIsIdentity) {
  using namespace sora;
  float data1[] = {1, 0, 0, 1};
  EXPECT_EQ(true, MatIsIdentity(data1));
  float data2[] = {1, 0, 1, 2};
  EXPECT_EQ(false, MatIsIdentity(data2));
}

TEST(Matrix, MatGetRow) {
  using namespace sora;
  //1 3
	//0 4
	float data1[] = {1, 0, 3, 4};

  float row_data_0[2];
  MatGetRow(0, data1, row_data_0);
  float row_data_1[2];
	MatGetRow(1, data1, row_data_1);

	
  float t1[] = { 1, 3 };
  float t2[] = { 0, 4 };
  EXPECT_EQ(0, memcmp(row_data_0, t1, sizeof(t1)));
  EXPECT_EQ(0, memcmp(row_data_1, t2, sizeof(t2)));
}

TEST(Matrix, MatMult) {
  using namespace sora;
  float data1[] = {1, 2,  3, 4};
  float data2[] = {1, 0,  0, 2};
  
  Mat2f m1;
  MatCopy(data1, m1);
  Mat2f m2;
  MatCopy(data2, m2);

  Mat2f result;
  MatMult(m1, m2, result);

  float data3[] = {1, 2,  6, 8};
  Mat2f m3;
  MatCopy(data3, m3);
  EXPECT_EQ(true, MatEq(m3, result));
}

TEST(Matrix, GetIndex) {
  using namespace sora;
  EXPECT_EQ(0, (GetIndex<2, 0, 0>()));
  EXPECT_EQ(2, (GetIndex<2, 0, 1>()));
  EXPECT_EQ(1, (GetIndex<2, 1, 0>()));
  EXPECT_EQ(3, (GetIndex<2, 1, 1>()));
}

