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
#ifndef BASE_MATSU_MATRIX_IMPL_H_
#define BASE_MATSU_MATRIX_IMPL_H_

namespace matsu {;
template <typename T>
MatrixTemplate<T, 4, 4> Matrix::Identity() {
  return MatrixTemplate<T, 4, 4>();
}
template <typename T>
MatrixTemplate<T, 4, 4> Matrix::Translate(T x, T y, T z) {
  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      x, y, z, 1
  };
  m.SetMatrix(data);
  return m;
}
template <typename T>
MatrixTemplate<T, 4, 4> Matrix::Scale(T s) {
  return Scale(s, s, s);
}
template<typename T>
MatrixTemplate<T, 4, 4> Matrix::Scale(T x, T y, T z) {
  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1
  };
  m.SetMatrix(data);
  return m;
}
template <typename T>
MatrixTemplate<T, 4, 4> Matrix::RotateX(T degrees) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);

  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      1, 0, 0, 0,
      0, c, s, 0,
      0, -s, c, 0,
      0, 0, 0, 1
  };
  m.SetMatrix(data);
  return m;
}
template <typename T>
MatrixTemplate<T, 4, 4> Matrix::RotateY(T degrees) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);

  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      c, 0, -s, 0,
      0, 1, 0, 0,
      s, 0, c, 0,
      0, 0, 0, 1,
  };
  m.SetMatrix(data);
  return m;
}
template <typename T>
MatrixTemplate<T, 4, 4> Matrix::RotateZ(T degrees) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);

  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      c, s, 0, 0,
      -s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  };
  m.SetMatrix(data);
  return m;
}

template<typename T>
MatrixTemplate<T, 4, 4> Matrix::xAxisShear(T y, T z) {
  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      1, 0, 0, 0,
      y, 1, 0, 0,
      z, 0, 1, 0,
      0, 0, 0, 1
  };
  m.SetMatrix(data);
  return m;
}
template<typename T>
MatrixTemplate<T, 4, 4> Matrix::yAxisShear(T x, T z) {
  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      1, x, 0, 0,
      0, 1, 0, 0,
      0, z, 1, 0,
      0, 0, 0, 1,
  };
  m.SetMatrix(data);
  return m;
}
template<typename T>
MatrixTemplate<T, 4, 4> Matrix::zAxisShear(T x, T y) {
  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      1, 0, x, 0,
      0, 1, y, 0,
      0, 0, 1, 0,
      0, 0, 0, 1,
  };
  m.SetMatrix(data);
  return m;
}

template<typename T>
MatrixTemplate<T, 4, 4> Matrix::Ortho(T left,
  T right, T bottom, T top, T nearVal, T farVal) {
  // http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
  T a = static_cast<T>(2) / (right - left);
  T b = static_cast<T>(2) / (top - bottom);
  T c = static_cast<T>(2) / (farVal - nearVal); //-2, 2에 따라서 이동방향이 바뀐다
  T tx = -(right+left) / (right-left);
  T ty = -(top+bottom) / (top-bottom);
  T tz = -(farVal+nearVal) / (farVal-nearVal);

  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      a, 0, 0, 0,
      0, b, 0, 0,
      0, 0, c, 0,
      tx, ty, tz, 1
  };
  m.SetMatrix(data);

  return m;
}

template<typename T>
MatrixTemplate<T, 4, 4> Matrix::Frustum(T left,
  T right, T bottom, T top, T nearVal, T farVal) {
  T a = static_cast<T>(2) * nearVal / (right - left);
  T b = static_cast<T>(2) * nearVal / (top - bottom);
  T c = (right + left) / (right - left);
  T d = (top + bottom) / (top - bottom);
  T e = - (farVal + nearVal) / (farVal - nearVal);
  T f = -static_cast<T>(2) * farVal * nearVal / (farVal - nearVal);
  MatrixTemplate<T, 4, 4> m;
  T data[] = {
      a, 0, 0, 0,
      0, b, 0, 0,
      c, d, e, -1,
      0, 0, f, 1
  };
  m.SetMatrix(data);
  return m;
}

template<typename T>
MatrixTemplate<T, 4, 4> Matrix::LookAt(const matsu::VectorTemplate<T, 3> &eye,
    const matsu::VectorTemplate<T, 3> &target,
    const matsu::VectorTemplate<T, 3> &up) {
  matsu::VectorTemplate<T, 3> z = (eye-target).normalize();
  matsu::VectorTemplate<T, 3> x = up.cross(z).normalize();
  matsu::VectorTemplate<T, 3> y = z.cross(x).normalize();

  MatrixTemplate<T, 4, 4> m;
  m.SetCol(0, vec4(x, 0));
  m.SetCol(1, vec4(y, 0));
  m.SetCol(2, vec4(z, 0));
  m.SetCol(3, vec4(0, 0, 0, 1));

  VectorTemplate<T, 4> eyePrime;
  eyePrime = m * matsu::VectorTemplate<T, 4>(-eye, static_cast<T> (1));
  m = m.Transpose();
  m.SetCol(3, eyePrime);

  return m;
}
}

#endif  // BASE_MATSU_MATRIX_IMPL_H_

