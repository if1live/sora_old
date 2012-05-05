// Copyright (C) 2011-2012 by if1live
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
#include "sys_var.h"

using namespace std;
using namespace sora;

TEST(SysVar, test) {
  SysVar str_var = SysVar::String("asdf");
  SysVar int_var = SysVar::Int(2);
  SysVar float_var = SysVar::Float(2.3f);

  EXPECT_EQ(SysVar::kSysVarString, str_var.type());
  EXPECT_STREQ("asdf", str_var.str().c_str());

  EXPECT_EQ(SysVar::kSysVarInt, int_var.type());
  EXPECT_STREQ("2", int_var.str().c_str());
  EXPECT_EQ(2, int_var.int_ref());

  EXPECT_EQ(SysVar::kSysVarFloat, float_var.type());
  EXPECT_STREQ("2.3000", float_var.str().c_str());
  EXPECT_EQ(2.3f, float_var.float_ref());
}

TEST(SysVarDictionary, test) {
  SysVarDictionary dict;
  EXPECT_EQ(true, dict.Add("key1", 1));
  EXPECT_EQ(true, dict.Add("key2", 2.04f));
  EXPECT_EQ(true, dict.Add("key3", "asdf"));

  //중복추가는 씹는다
  EXPECT_EQ(false, dict.Add("key3", 1));

  EXPECT_EQ(true, dict.IsExist("key1"));
  EXPECT_EQ(false, dict.IsExist("key1dsfdsrf"));

  SysVar *var1 = dict.Get("key1");
  EXPECT_EQ(true, var1 != NULL);
  SysVar *var2 = dict.Get("fgedsfdsf");
  EXPECT_EQ(true, var2 == NULL);

}