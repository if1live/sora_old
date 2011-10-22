﻿/*  Copyright (C) 2011 by if1live */
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
#ifndef BASE_MATSU_RANGE_H_
#define BASE_MATSU_RANGE_H_

#include "sora/macro.h"

namespace matsu {;
template<typename T>
class Range {
 public:
  static const Range &ZeroOne() {
    static Range r(0, 1);
    return r;
  }
  static const Range &PlusMinus05() {
    T min_value = static_cast<T>(-0.5);
    T max_value = -min_value;
    static Range r(min_value, max_value);
    return r;
  }

 public:
  explicit Range(T v1, T v2) 
    : minimum(0), maximum(0) {
      if (v1 < v2) {
        minimum = v1;
        maximum = v2;
      } else {
        minimum = v2;
        maximum = v1;
      }
  }
  ~Range() {}
  T Clamp(T value) {
    if(value < minimum) {
      return minimum;
    } else if(value > maximum) {
      return maximum;
    } else {
      return value;
    }
  }
public:
  T minimum;
  T maximum;
};
}

#endif  // BASE_MATSU_RANGE_H_
