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
#include "str_code.h"

using namespace std;
using namespace sora;

TEST(StrCode, test) {
  StrCode code1("asdf");
  StrCode code2(std::string("asdf"));
  StrCode code3(std::string("asdffds"));

  EXPECT_TRUE(code1 == code2);
  EXPECT_TRUE(code3 != code2);
}

TEST(StrCodeDictionary, test) {
  StrCodeDictionary dict;
  SCode code1 = dict.Get("a");
  SCode code2 = dict.Get("bfdsfds");
  SCode code3 = dict.Get("a");
  EXPECT_EQ(code1, code3);
  EXPECT_NE(code1, code2);

  EXPECT_EQ(2, dict.count());
  EXPECT_EQ(true, dict.IsExist("a"));
  EXPECT_EQ(false, dict.IsExist("a43"));
}