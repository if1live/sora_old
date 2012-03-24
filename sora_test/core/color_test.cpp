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
#include "core/color.h"

using namespace sora;
TEST(Color, ColorGetMaxValue) {
  EXPECT_EQ(1.0f, ColorGetMaxValue<float>());
  EXPECT_EQ(255, ColorGetMaxValue<uchar>());

  Color3ub c1;
  EXPECT_EQ(255, ColorGetMaxValue(c1));
  Color4f c2;
  EXPECT_EQ(1.0f, ColorGetMaxValue(c2));
}

TEST(Color, ElemCount) {
  using sora::Color4f;
  Color4f c1;
  EXPECT_EQ(4, ColorElemCount(c1));
}

TEST(Color, ColorEq) {
  using sora::Color3ub;
  using sora::Color4ub;
  uchar c1[] = { 1, 2, 3 };
  uchar c2[] = { 1, 2, 3 };
  uchar c3[] = { 1, 2, 4 };
  uchar c4[] = { 1, 2, 3, 255 };
  uchar c5[] = { 1, 2, 3, 212 };
  EXPECT_EQ(true, ColorEq(c1, c2));
  EXPECT_EQ(true, ColorNotEq(c1, c3));
  EXPECT_EQ(true, ColorEq(c1, c4));
  EXPECT_EQ(true, ColorEq(c4, c1));
  EXPECT_EQ(true, ColorNotEq(c1, c5));
}

TEST(Color, construct) {
  using sora::Color3ub;
  using sora::Color4ub;
  using sora::Color3f;
  {
    // color3ub -> color4ub(up casting)
    Color3ub c1 = { 10, 20, 30 };
    Color4ub c2;
    ColorCopy(c1, c2);

    EXPECT_EQ(10, c2[0]);
    EXPECT_EQ(20, c2[1]);
    EXPECT_EQ(30, c2[2]);
    EXPECT_EQ(255, c2[3]);
  }
  {
    // color4ub -> color3ub(down casting)
    Color4ub c1 = { 1, 2, 3, 4 };
    Color3ub c2;
    ColorCopy(c1, c2);

    EXPECT_EQ(1, c2[0]);
    EXPECT_EQ(2, c2[1]);
    EXPECT_EQ(3, c2[2]);
    //EXPECT_EQ(255, c2.alpha());
  }
  {
    // type convert
    Color3ub c1 = { 10, 20, 30 };
    Color3f c2;
    ColorCopy(c1, c2);

    EXPECT_EQ(10.0f/255.0f, c2[0]);
  }
}