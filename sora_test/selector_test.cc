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
#include "sora/selector.h"

class TestSelector1 : public sora::Selector {
public:
  TestSelector1() : test_value1(0) {}
  virtual ~TestSelector1() {}

  virtual void TestFunc() { test_value1++; }
public:
  int test_value1;
};

TEST(Selector, CallFunc) {
  using namespace sora;

  TestSelector1 sel1;
  EXPECT_EQ(0, sel1.test_value1);
  Selector_CallFunc func = SR_CALL_FUNC_SEL(TestSelector1::TestFunc);

  Selector *sel = &sel1;
  (sel->*func)();
  EXPECT_EQ(1, sel1.test_value1);

  ((&sel1)->*func)();
  EXPECT_EQ(2, sel1.test_value1);
}