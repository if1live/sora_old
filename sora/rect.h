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

  void Init(T x, T y, T w, T h) {
    data[0] = x;
    data[1] = y;
    data[2] = w;
    data[3] = h;
  }

  void GetOrigin(T (&p)[2]) {
    p[0] = x;
    p[1] = y;
  }
  void GetSize(T (&p)[2]) {
    p[0] = width;
    p[1] = height;
  }

  bool operator==(const Rect &o) const {
    if(memcmp(data, o.data, sizeof(T) * 4) == 0) {
      return true;
    } else {
      return false;
    }
  }

  bool operator!=(const Rect &o) const {
    return !(*this == o);
  }

  bool IsEmpty() const {
    Rect empty;
    return (*this == empty);
  }

};

}

#endif  // SORA_RECT_H_