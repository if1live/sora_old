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
#include "precompile.h"
#include "matsu/range.h"

TEST(Range, Clamp) {
  using matsu::Range;
  Range<float> r1(-1, 2);
  EXPECT_EQ(-1, r1.Clamp(-2));
  EXPECT_EQ(2, r1.Clamp(4));
  EXPECT_EQ(0.4f, r1.Clamp(0.4f));

  Range<float> r2(2, -1);
  EXPECT_EQ(-1, r2.Clamp(-2));
  EXPECT_EQ(2, r2.Clamp(4));
  EXPECT_EQ(0.4f, r2.Clamp(0.4f));
}