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
#ifndef BASE_MATSU_MATRIX_H_
#define BASE_MATSU_MATRIX_H_

#include "matsu/matrix_template.h"

namespace matsu {;
typedef MatrixTemplate<int, 2, 2> imat2;
typedef MatrixTemplate<float, 2, 2> mat2;
typedef MatrixTemplate<int, 3, 3> imat3;
typedef MatrixTemplate<float, 3, 3> mat3;
typedef MatrixTemplate<int, 4, 4> imat4;
typedef MatrixTemplate<float, 4, 4> mat4;
}

namespace matsu {;
class Matrix {
 public:
  template<typename T>
  static MatrixTemplate<T, 4, 4> Identity();

  template<typename T>
  static MatrixTemplate<T, 4, 4> Translate(T x, T y, T z);

  template<typename T>
  static MatrixTemplate<T, 4, 4> Scale(T s);

  template<typename T>
  static MatrixTemplate<T, 4, 4> Scale(T x, T y, T z);

  template<typename T>
  static MatrixTemplate<T, 4, 4> RotateX(T degrees);

  template<typename T>
  static MatrixTemplate<T, 4, 4> RotateY(T degrees);

  template<typename T>
  static MatrixTemplate<T, 4, 4> RotateZ(T degrees);

  template<typename T>
  static MatrixTemplate<T, 4, 4> xAxisShear(T y, T z);

  template<typename T>
  static MatrixTemplate<T, 4, 4> yAxisShear(T x, T z);

  template<typename T>
  static MatrixTemplate<T, 4, 4> zAxisShear(T z, T y);

  template<typename T>
  static MatrixTemplate<T, 4, 4> Ortho(T left,
    T right, T bottom, T top, T nearVal, T farVal);

  template<typename T>
  static MatrixTemplate<T, 4, 4> Frustum(T left,
    T right, T bottom, T top, T nearVal, T farVal);

  template<typename T>
  static MatrixTemplate<T, 4, 4> LookAt(
    const matsu::VectorTemplate<T, 3> &eye,
    const matsu::VectorTemplate<T, 3> &target,
    const matsu::VectorTemplate<T, 3> &up);
};
}

#include "matsu/matrix_impl.h"

#endif  // BASE_MATSU_MATRIX_H_

