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
#ifndef SORA_RECT_H_
#define SORA_RECT_H_

#include "vector.h"
#include <cstring>

namespace sora {;
//x, y, width, height
template<typename T>  struct Rect;
typedef Rect<float> Rectf;
typedef Rect<int> Recti;

template<typename T>  
struct Rect {
  union {
    struct { T x, y, width, height; };
    T data[4];
  };

  Rect() { memset(data, 0, sizeof(T) * 4); }
  Rect(T x, T y, T w, T h) : x(x), y(y), width(w), height(h) {}
  ~Rect() {}
};

template<typename T>
void RectInit(T x, T y, T w, T h, T (&rect)[4]) {
  rect[0] = x;
  rect[1] = y;
  rect[2] = w;
  rect[3] = h;
}
template<typename T>
void RectCopy(const T (&src)[4], T (&dst)[4]) {
  memcpy(dst, src, sizeof(T) * 4);
}

template<typename T>
void RectGetOrigin(const T (&rect)[4], T (&p)[2]) {
  p[0] = rect[0];
  p[1] = rect[1];
}
template<typename T>
void RectGetSize(const T (&rect)[4], T (&p)[2]) {
  p[0] = rect[2];
  p[1] = rect[3];
}

template<typename T>
T RectGetX(const T (&rect)[4]) { return rect[0]; }
template<typename T>
T RectGetY(const T (&rect)[4]) { return rect[1]; }
template<typename T>
T RectGetWidth(const T (&rect)[4]) { return rect[2]; }
template<typename T>
T RectGetHeight(const T (&rect)[4]) { return rect[3]; }

template<typename T>
bool RectEq(const T (&a)[4], const T (&b)[4]) {
  if(memcmp(a, b, sizeof(T) * 4) == 0) {
    return true;
  } else {
    return false;
  }
}

template<typename T>
bool RectNotEq(const T (&a)[4], const T (&b)[4]) {
  return !(RectEq(a, b));
}

template<typename T>
bool RectIsEmpty(const T (&rect)[4]) const {
  T empty[4];
  memset(empty, 0, sizeof(empty);
  return RectEq(rect, empty);
}

}

#endif  // SORA_RECT_H_