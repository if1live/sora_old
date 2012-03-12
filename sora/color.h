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
#ifndef SORA_COLOR_H_
#define SORA_COLOR_H_

#include <cstring>
#include "arch.h"
#include "assert_inc.h"

namespace sora {;
typedef float Color3f[3];
typedef float Color4f[4];
typedef uchar Color3ub[3];
typedef uchar Color4ub[4];

template<typename T>
T ColorGetMaxValue() { return 1; }
template<>
float ColorGetMaxValue() { return 1.0f; }
template<>
uchar ColorGetMaxValue() { return 255; }

template<typename T, int D>
T ColorGetMaxValue(const T (&c)[D]) {
  return ColorGetMaxValue<T>();
}
template<typename T, int D>
int ColorElemCount(const T (&c)[D]) {
  return D;
}

template<typename T, int D>
void ColorSet(T r, T g, T b, const T (&c)[D]) {
  SR_STATIC_ASSERT(D >= 3 && D <= 4, "not valid color");
  c[0] = r;
  c[1] = g;
  c[2] = b;
  if(D == 4) {
    c[3] = 0;
  }
}
template<typename T, int D>
void ColorSet(T r, T g, T b, T a, const T (&c)[D]) {
  SR_STATIC_ASSERT(D >= 3 && D <= 4, "not valid color");
  c[0] = r;
  c[1] = g;
  c[2] = b;
  if(D == 4) {
    c[4] = a;
  }
}

template<typename T, int D>
void ColorCopy(const T (&src)[D], T (&dst)[D]) {
  memcpy(dst, src, sizeof(T) * D);
}
template<typename T1, typename T2, int D>
void ColorCopy(const T1 (&src)[D], T2 (&dst)[D]) {
  T2 t1_max = static_cast<T2>(ColorGetMaxValue<T1>());
  T2 t2_max = ColorGetMaxValue<T2>();
  for(int i = 0 ; i < D ; i++) {
    dst[i] = static_cast<T2>(src[i] / t1_max * t2_max);
  }
}

template<typename T, int D1, int D2>
void ColorCopy(const T (&src)[D1], T (&dst)[D2]) {
  SR_STATIC_ASSERT(D1 >= 3 && D1 <= 4, "not valid color");
  SR_STATIC_ASSERT(D2 >= 3 && D2 <= 4, "not valid color");
  SR_STATIC_ASSERT(D1 != D2, "maybe not this function");
  if(D1 == 3 && D2 == 4) {
    memcpy(dst, src, sizeof(T) * 3);
    dst[3] = ColorGetMaxValue<T>();
  } else if(D1 == 4 && D2 == 3) {
    memcpy(dst, src, sizeof(T) * 3);
  }
}

template<typename T, int D>
bool ColorEq(const T (&a)[D], const T (&b)[D]) {
  if(memcmp(a, b, sizeof(T) * D) == 0) {
    return true;
  } else {
    return false;
  }
}
template<typename T, int D1, int D2>
bool ColorEq(const T (&a)[D1], const T (&b)[D2]) {
  SR_STATIC_ASSERT(D1 >= 3 && D1 <= 4, "not valid color");
  SR_STATIC_ASSERT(D2 >= 3 && D2 <= 4, "not valid color");
  SR_STATIC_ASSERT(D1 != D2, "maybe not this function");

  if(memcmp(a, b, sizeof(T) * 3) != 0) {
    return false;
  }
  //check alpah
  if(D1 == 4 && D2 == 3) {
    if(a[3] == ColorGetMaxValue<T>()) {
      return true;
    } else {
      return false;
    }
  } else if(D1 == 3 && D2 == 4) {
    if(b[3] == ColorGetMaxValue<T>()) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

template<typename T, int D>
bool ColorNotEq(const T (&a)[D], const T (&b)[D]) {
  return !(ColorEq(a, b));
}
template<typename T, int D1, int D2>
bool ColorNotEq(const T (&a)[D1], const T (&b)[D2]) {
  return !(ColorEq(a, b));
}

};

#endif  // SORA_COLOR_H_