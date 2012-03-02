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
#include "sora/common/template_library.h"

struct CtorDtorSample {
  CtorDtorSample() { value = 1; }
  ~CtorDtorSample() { value = 0;}
  static int value;
};
int CtorDtorSample::value = -1;

TEST(CallConstructor_CallDestructor, test) {
  using namespace sora;
  EXPECT_EQ(-1, CtorDtorSample::value);

  CtorDtorSample *ptr = (CtorDtorSample*)malloc(sizeof(CtorDtorSample));
  CallConstructor(ptr);
  EXPECT_EQ(1, CtorDtorSample::value);

  CallDestructor(ptr);
  free(ptr);
  EXPECT_EQ(0, CtorDtorSample::value);
}