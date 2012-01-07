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
#ifndef SORA_COLOR_H_
#define SORA_COLOR_H_

namespace sora {;

template<typename T>  struct Color3;
template<typename T>  struct Color4;
typedef Color3<int>  Color3ub;
typedef Color3<int>  Color3f;
typedef Color4<float>  Color4ub;
typedef Color4<float>  Color4f;

template<typename T>
struct Color3 {
  enum {
    ElemCount = 3,
  };
  union {
    struct { T r; T g; T b; };
    T value[ElemCount];
  };
  Color3() {
    for (int i = 0 ; i < ElemCount ; i++) {
      value[i] = max_value();
    }
  }
  Color3(T v) : r(v), g(v), b(b) {}

  static T max_value() { return 255; }
  static T min_value() { return 0; }

  static bool HasAlpha() { return false; }
};

template<typename T>
struct Color4 {
  enum {
    ElemCount = 4,
  };
  union {
    struct { T r; T g; T b; T a; };
    T value[ElemCount];
  };

  Color4() {
    for (int i = 0 ; i < ElemCount ; i++) {
      value[i] = max_value();
    }
  }
  Color4(T v) : r(v), g(v), b(b), a(v) {}
  Color4(T r, T g, T b) : r(r), g(g), b(b), a(max_value()) {}
  Color4(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}

  static T max_value() { return 255; }
  static T min_value() { return 0; }

  static bool HasAlpha() { return true; }
};

//float 일때는 max가 바뀐다
template<>
float Color3<float>::max_value() { return 1.0f; }

template<>
float Color4<float>::max_value() { return 1.0f; }

template<typename ColorType>
void ToRed(ColorType &color) {
  color.r = ColorType::max_value();
  color.g = 0;
  color.b = 0;
  if (ColorType::HasAlpha()) {
    color.value[3] = ColorType::max_value();
  }
}
template<typename ColorType>
void ToGreen(ColorType &color) {
  color.r = 0;
  color.g = ColorType::max_value();
  color.b = 0;
  if (ColorType::HasAlpha()) {
    color.value[3] = ColorType::max_value();
  }
}

}

#endif  // SORA_COLOR_H_