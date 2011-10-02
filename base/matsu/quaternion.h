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
#ifndef BASE_MATSU_QUATERNION_H_
#define BASE_MATSU_QUATERNION_H_

#include "matsu/vector.h"
#include "matsu/matrix.h"

namespace matsu {;
template<typename T>  class QuaternionTemplate;
typedef QuaternionTemplate<float> Quaternion;   // 일반적으로 쓸거같은거
typedef QuaternionTemplate<float> QuaternionF;
typedef QuaternionTemplate<float> QuaternionD;
typedef QuaternionTemplate<float> quat;

template <typename T>
class QuaternionTemplate {
 public:
  static QuaternionTemplate<T> CreateFromVectors(
    const VectorTemplate<T, 3>& v0, const VectorTemplate<T, 3>& v1);
  static QuaternionTemplate<T> CreateFromAxisAngle(
    const VectorTemplate<T, 3>& axis, float radians);

 public:
  T x;
  T y;
  T z;
  T w;

  QuaternionTemplate();
  QuaternionTemplate(T x, T y, T z, T w);

  QuaternionTemplate<T> Slerp(T mu, const QuaternionTemplate<T>& q) const;
  QuaternionTemplate<T> Rotated(const QuaternionTemplate<T>& b) const;
  QuaternionTemplate<T> Scaled(T scale) const;
  T Dot(const QuaternionTemplate<T>& q) const;
  MatrixTemplate<T, 3, 3> ToMatrix() const;
  VectorTemplate<T, 4> ToVector() const;
  QuaternionTemplate<T> operator-(const QuaternionTemplate<T>& q) const;
  QuaternionTemplate<T> operator+(const QuaternionTemplate<T>& q) const;
  bool operator==(const QuaternionTemplate<T>& q) const;
  bool operator!=(const QuaternionTemplate<T>& q) const;

  QuaternionTemplate<T> operator*(const QuaternionTemplate<T> &ro) const;
  VectorTemplate<T, 3> operator*(const VectorTemplate<T, 3> &v) const;

  void Normalized();
  void Rotate(const QuaternionTemplate<T>& q);

  QuaternionTemplate<T> Conjugation() const;
  QuaternionTemplate<T> Inverse() const;
};
}

#include "matsu/quaternion_impl.h"

#endif  // BASE_MATSU_QUATERNION_H_
