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
#include "sora/string_table.h"
#include "sora/string_hash_functor.h"

TEST(StringTable, test) {
  using namespace sora;
  using std::string;

  StringTable<RSHash> table;

  string str1("fsd");
  string str2("4ef");

  unsigned int code1 = table.ToCode(str1);
  unsigned int code2 = table.ToCode(str2);
  EXPECT_EQ(true, code1 != code2);

  table.Save(str1);
  EXPECT_EQ(true, table.IsExist(str1));
  EXPECT_EQ(false, table.IsExist(str2));
  EXPECT_EQ(true, table.IsExist(code1));
  EXPECT_EQ(false, table.IsExist(code2));

  EXPECT_EQ(true, str1 == table.Load(code1));
}