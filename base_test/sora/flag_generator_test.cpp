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
#include "sora/flag_generator.h"

using sora::UintFlagGenerator;
using std::string;

TEST(FlagGenerator, TEST) {
  UintFlagGenerator generator;
  EXPECT_EQ(0, generator.Count());

  string key1 = "fdshrow";
  string key2 = "fdr23";
  string key3 = "243";
  unsigned int flag1 = generator.Get(key1);
  unsigned int flag2 = generator.Get(key2);
  unsigned int flag3 = generator.Get(key3);

  EXPECT_EQ(0x01, flag1);
  EXPECT_EQ(0x02, flag2);
  EXPECT_EQ(0x04, flag3);
  EXPECT_EQ(3, generator.Count());

  EXPECT_EQ(flag2, generator.Get(key2));
  EXPECT_EQ(true, generator.IsExist(key2));

}