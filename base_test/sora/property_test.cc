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
#include <string>
#include "sora/property.h"

namespace dummy {;
class PropertySample {
public:
  PropertySample() : c(0) {}

  int get_a() {
    return a.value * 2;
  }
  int set_a(const int &a) {
    return a * 2;
  }

  int get_c() {
    return c.value * 3;
  }
  int set_c(const int &c) {
    return c * 3;
  }
  
  std::string get_str() {
    return std::string("a");
  }
  std::string set_str(const std::string &str) {
    return str + "a";
  }

  SR_PROPERTY(int, PropertySample, get_c, set_c, c);
  SR_PROPERTY(std::string, PropertySample, get_str, set_str, d);

public:
  //normal form
  class {
  public:
    int &operator=(const int &i) {
      unsigned int offset = offsetof(PropertySample, a);
      PropertySample *parent = (PropertySample*)((unsigned int)(this) - offset);
      value = parent->set_a(i);
      return value;
    }
    operator int() const {
      unsigned int offset = offsetof(PropertySample, a);
      PropertySample *parent = (PropertySample*)((unsigned int)(this) - offset);
      return parent->get_a();
    }
    int value;
  } a;

  //legacy style
  class {
  public:
    float &operator=(const float &i) { return value = i; }
    operator float() const { return value; }
    float value;
  } b;
};
}

TEST(Property, test) {
  dummy::PropertySample p1;
  p1.a = 1;   //set하면 2배가 저장
  EXPECT_EQ(4, p1.a);   //get할때 다시 2배

  p1.b = 2;
  EXPECT_EQ(2, p1.b);

  //기본값 설정 지원
  EXPECT_EQ(0, p1.c);
  p1.c = 1;   //set하면 3배가 저장
  EXPECT_EQ(9, p1.c);   //get할때 다시 3배

  //객체에 대해서 테스트
  p1.d = "msg";
  std::string tmp = p1.d;
  EXPECT_EQ(true, tmp == "a");
}