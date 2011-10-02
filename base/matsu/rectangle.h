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
#ifndef BASE_MATSU_RECTANGLE_H_
#define BASE_MATSU_RECTANGLE_H_

namespace matsu {;
template<typename T> class Rectangle;
typedef Rectangle<int> Recti;
typedef Rectangle<float> Rectf;

template<typename T>
class Rectangle {
 public:
  typedef T Type;
 public:
  Rectangle(T x, T y, T w, T h);
  ~Rectangle();

  T x;
  T y;
  T width;
  T height;
};
}

namespace matsu {;
template<typename T>
Rectangle<T>::Rectangle(T x, T y, T w, T h)
  : x(x), y(y), width(w), height(h) {
}
template<typename T>
Rectangle<T>::~Rectangle() {
}
}
#endif  // BASE_MATSU_RECTANGLE_H_

