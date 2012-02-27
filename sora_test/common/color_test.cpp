/*  Copyright (C) 2011 by if1live */
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
#include "sora/common/color.h"

TEST(Color3ub, test) {
  using sora::Color3ub;
  Color3ub c1;
  EXPECT_EQ(255, c1.r);
  EXPECT_EQ(255, c1.g);
  EXPECT_EQ(255, c1.b);

  Color3ub c2(255, 255, 0);
  EXPECT_EQ(255, c2.r);
  EXPECT_EQ(255, c2.g);
  EXPECT_EQ(0, c2.b);

  EXPECT_EQ(255, Color3ub::max_value());
  EXPECT_EQ(0, Color3ub::min_value());

  //alpha test
  //c2.set_alpha(23);
  //EXPECT_EQ(255, c2.alpha());
}

TEST(Color3f, test) {
  using sora::Color3f;
  Color3f c1;
  EXPECT_EQ(1.0f, c1.r);
  EXPECT_EQ(1.0f, c1.g);
  EXPECT_EQ(1.0f, c1.b);

  Color3f c2(0.1f, 0.2f, 1.3f);
  ColorValidate(&c2);
  EXPECT_EQ(0.1f, c2.r);
  EXPECT_EQ(0.2f, c2.g);
  EXPECT_EQ(1.0f, c2.b);  // 범위 넘어가면 보정

  EXPECT_EQ(1.0f, Color3f::max_value());
  EXPECT_EQ(0.0f, Color3f::min_value());

  EXPECT_EQ(sizeof(Color3f), sizeof(float) * 3);

  //alpha test
  //c2.set_alpha(0.3f);
  //EXPECT_EQ(1.0f, c2.alpha());
}


TEST(Color4f, test) {
  using sora::Color4f;
  Color4f c1;
  EXPECT_EQ(sizeof(Color4f), sizeof(float) * 4);

  //alpha test
  //c1.set_alpha(0.3f);
  //EXPECT_EQ(0.3f, c1.alpha());
}

TEST(Color, ElemCount) {
  using sora::Color4f;
  Color4f c1;
  EXPECT_EQ(4, c1.ElemCount);
  EXPECT_EQ(4, Color4f::ElemCount);
}

TEST(Color, construct) {
  using sora::Color3ub;
  using sora::Color4ub;
  using sora::Color3f;
  {
    // color3ub -> color4ub(up casting)
    Color3ub c1(10, 20, 30);
    Color4ub c2(c1);
    EXPECT_EQ(10, c2.r);
    EXPECT_EQ(20, c2.g);
    EXPECT_EQ(30, c2.b);
    //EXPECT_EQ(255, c2.alpha());
  }
  {
    // color4ub -> color3ub(down casting)
    Color4ub c1(1, 2, 3, 4);
    Color3ub c2(c1);
    EXPECT_EQ(1, c2.r);
    EXPECT_EQ(2, c2.g);
    EXPECT_EQ(3, c2.b);
    //EXPECT_EQ(255, c2.alpha());
  }
  {
    // type convert
    Color3ub c1(10, 20, 30);
    Color3f c2(c1);
    EXPECT_EQ(10.0f/255.0f, c2.r);
    Color3ub c3(c2);
    EXPECT_EQ(true, c3 == c1);
  }
}

TEST(Color, eq) {
  using sora::Color3ub;
  Color3ub c1(1, 2, 3);
  Color3ub c2(1, 2, 3);
  Color3ub c3(1, 2, 4);
  EXPECT_EQ(true, c1 == c2);
  EXPECT_EQ(true, c1 != c3);
}