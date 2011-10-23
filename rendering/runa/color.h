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
#ifndef RENDERING_RUNA_COLOR_H_
#define RENDERING_RUNA_COLOR_H_

#include "runa/runa_enum.h"
#include <boost/static_assert.hpp>

namespace runa {;
template<typename T, unsigned int D>
struct Color {
  enum {
    Dimension = D
  };
  typedef T VarType;

  union {
    //struct 표기법에서 alpha는 짤라냈다. alpha는 함수로 접근하자
    struct {
        T r;
        T g;
        T b;
      };
      T data[D];
  };
  Color() {
    BOOST_STATIC_ASSERT(D == 3 || D == 4);
    for (unsigned int i = 0 ; i < D ; i++) {
      data[i] = MaxValue();
    }
  }
  Color(T r, T g, T b) {
    Set(r, g, b);
  }
  Color(T r, T g, T b, T a) {
    Set(r, g, b, a);
  }
  template<unsigned int D2>
  Color(const Color<T, D2> &o) {
    Set(o.r, o.g, o.b, o.alpha());
  }
  template<typename T2>
  Color(const Color<T2, D> &o) {
    T r = o.r / (float)o.MaxValue() * MaxValue();
    T g = o.g / (float)o.MaxValue() * MaxValue();
    T b = o.b / (float)o.MaxValue() * MaxValue();
    T a = o.alpha() / (float)o.MaxValue() * MaxValue();
    Set(r, g, b, a);
  }
  void Set(T r, T g, T b, T a) {
    if (r < MinValue()) {
      this->r = MinValue();
    } else if(r > MaxValue()) {
      this->r = MaxValue();
    } else {
      this->r = r;
    }

    if (g < MinValue()) {
      this->g = MinValue();
    } else if(g > MaxValue()) {
      this->g = MaxValue();
    } else {
      this->g = g;
    }

    if (b < MinValue()) {
      this->b = MinValue();
    } else if(b > MaxValue()) {
      this->b = MaxValue();
    } else {
      this->b = b;
    }

    if (D == 4) {
      if (a < MinValue()) {
        data[3] = MinValue();
      } else if(a > MaxValue()) {
        data[3] = MaxValue();
      } else {
        data[3] = a;
      }
    }
  }
  void Set(T r, T g, T b) {
    Set(r, g, b, MaxValue());
  }
  bool operator==(const Color &o) {
    if( memcmp(data, o.data, sizeof(T) * D) == 0) {
      return true;
    } else {
      return false;
    }
  }
  bool operator!=(const Color &o) {
    return !(*this == o);
  }
  static T MinValue() { return 0; }
  static T MaxValue() { return static_cast<T>(1.0f); }

  // for alpha
  T alpha() const {
    if (D == 3) {
      return MaxValue();
    } else {
      return data[3];
    }
  }
  void set_alpha(T a) {
    if (D == 4) {
      data[3] = a;
    }
  }
  static const Color &White() {
    static Color color(MaxValue(), MaxValue(), MaxValue(), MaxValue());
    return color;
  }
  static const Color &Black() {
    static Color color(0, 0, 0, MaxValue());
    return color;
  }
  static const Color &Red() {
    static Color color(MaxValue(), 0, 0, MaxValue());
    return color;
  }
  static const Color &Green() {
    static Color color(0, MaxValue(), 0, MaxValue());
    return color;
  }
  static const Color &Blue() {
    static Color color(0, 0, MaxValue(), MaxValue());
    return color;
  }
};

template<>
unsigned char Color<unsigned char, 3>::MaxValue() {
  return 255;
}
template<>
unsigned char Color<unsigned char, 4>::MaxValue() {
  return 255;
}
}

#endif  // RENDERING_RUNA_COLOR_H_