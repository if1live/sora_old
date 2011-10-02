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
#ifndef BASE_MATSU_QUATERNION_IMPL_H_
#define BASE_MATSU_QUATERNION_IMPL_H_

#include "matsu/math.h"

namespace matsu {;
template <typename T>
inline QuaternionTemplate<T>::QuaternionTemplate()
  : x(0), y(0), z(0), w(1) {
}

template <typename T>
inline QuaternionTemplate<T>::QuaternionTemplate(T x, T y, T z, T w)
  : x(x), y(y), z(z), w(w) {
}

// Ken Shoemake's famous method.
template <typename T>
inline QuaternionTemplate<T> QuaternionTemplate<T>::Slerp(T t,
  const QuaternionTemplate<T>& v1) const {
  using std::cos;
  using std::sin;

  const T epsilon = 0.0005f;
  T dotvalue = dot(v1);

  if (dotvalue > 1 - epsilon) {
    QuaternionTemplate<T> result = v1 + (*this - v1).scaled(t);
    result.Normalized();
    return result;
  }

  if (dotvalue < 0) {
    dotvalue = 0;
  }

  if (dotvalue > 1) {
    dotvalue = 1;
  }

  T theta0 = std::acos(dotvalue);
  T theta = theta0 * t;

  QuaternionTemplate<T> v2 = (v1 - scaled(dotvalue));
  v2.Normalize();

  QuaternionTemplate<T> q = scaled(cos(theta)) + v2.scaled(sin(theta));
  q.Normalize();
  return q;
}

template <typename T>
inline QuaternionTemplate<T> QuaternionTemplate<T>::Rotated(
  const QuaternionTemplate<T>& b) const {
  QuaternionTemplate<T> q;
  q.w = w * b.w - x * b.x - y * b.y - z * b.z;
  q.x = w * b.x + x * b.w + y * b.z - z * b.y;
  q.y = w * b.y + y * b.w + z * b.x - x * b.z;
  q.z = w * b.z + z * b.w + x * b.y - y * b.x;
  q.Normalize();
  return q;
}

template <typename T>
inline QuaternionTemplate<T> QuaternionTemplate<T>::Scaled(T s) const {
  return QuaternionTemplate<T>(x * s, y * s, z * s, w * s);
}

template <typename T>
inline T QuaternionTemplate<T>::Dot(const QuaternionTemplate<T>& q) const {
  return x * q.x + y * q.y + z * q.z + w * q.w;
}

template <typename T>
inline MatrixTemplate<T, 3, 3> QuaternionTemplate<T>::ToMatrix() const {
  const T s = 2;
  T xs, ys, zs;
  T wx, wy, wz;
  T xx, xy, xz;
  T yy, yz, zz;

  xs = x * s;
  ys = y * s;
  zs = z * s;

  wx = w * xs;
  wy = w * ys;
  wz = w * zs;

  xx = x * xs;
  xy = x * ys;
  xz = x * zs;

  yy = y * ys;
  yz = y * zs;
  zz = z * zs;

  MatrixTemplate<T, 3, 3> m;
  m.x.x = 1 - (yy + zz);
  m.y.x = xy - wz;
  m.z.x = xz + wy;

  m.x.y = xy + wz;
  m.y.y = 1 - (xx + zz);
  m.z.y = yz - wx;

  m.x.z = xz - wy;
  m.y.z = yz + wx;
  m.z.z = 1 - (xx + yy);
  return m;
}

template <typename T>
inline VectorTemplate<T, 4> QuaternionTemplate<T>::ToVector() const {
  return VectorTemplate<T, 4>(x, y, z, w);
}

template <typename T>
QuaternionTemplate<T> QuaternionTemplate<T>::operator-(
  const QuaternionTemplate<T>& q) const {
  return QuaternionTemplate<T>(x - q.x, y - q.y, z - q.z, w - q.w);
}

template <typename T>
QuaternionTemplate<T> QuaternionTemplate<T>::operator+(
  const QuaternionTemplate<T>& q) const {
  return QuaternionTemplate<T>(x + q.x, y + q.y, z + q.z, w + q.w);
}

template <typename T>
bool QuaternionTemplate<T>::operator==(const QuaternionTemplate<T>& q) const {
  return x == q.x && y == q.y && z == q.z && w == q.w;
}

template <typename T>
bool QuaternionTemplate<T>::operator!=(const QuaternionTemplate<T>& q) const {
  return !(*this == q);
}

template<typename T>
QuaternionTemplate<T> QuaternionTemplate<T>::operator*(
  const QuaternionTemplate<T> &rq) const {
  // the constructor takes its arguments as (x, y, z, w)
  return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
      w * rq.y + y * rq.w + z * rq.x - x * rq.z,
      w * rq.z + z * rq.w + x * rq.y - y * rq.x,
      w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

template<typename T>
VectorTemplate<T, 3> QuaternionTemplate<T>::operator*(
  const VectorTemplate<T, 3> &v) const {
  // http://gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation#Quaternion_from_axis-angle
  VectorTemplate<T, 3> vn(v);
  vn.normalized();

  QuaternionTemplate<T> vecQuat;
  QuaternionTemplate<T> resQuat;
  vecQuat.x = vn.getX();
  vecQuat.y = vn.getY();
  vecQuat.z = vn.getZ();
  vecQuat.w = 0.0f;

  resQuat = vecQuat * conjugation();
  resQuat = *this * resQuat;

  return VectorTemplate<T, 3>(resQuat.x, resQuat.y, resQuat.z);
}

// Compute the quaternion that rotates from a to b,
// avoiding numerical instability.
// Taken from "The Shortest Arc Quaternion"
// by Stan Melax in "Game Programming Gems".
template <typename T>
inline QuaternionTemplate<T> QuaternionTemplate<T>::CreateFromVectors(
  const VectorTemplate<T, 3>& v0, const VectorTemplate<T, 3>& v1) {
  if (v0 == -v1)   {
    return QuaternionTemplate<T>::CreateFromAxisAngle(vec3(1, 0, 0), kPi);
  }

  VectorTemplate<T, 3> c = v0.cross(v1);
  T d = v0.dot(v1);
  T s = std::sqrt((1 + d) * 2);

  QuaternionTemplate<T> q;
  q.x = c.x / s;
  q.y = c.y / s;
  q.z = c.z / s;
  q.w = s / 2.0f;
  return q;
}

template <typename T>
inline QuaternionTemplate<T>  QuaternionTemplate<T>::CreateFromAxisAngle(
  const VectorTemplate<T, 3>& axis, float radians) {
  // http://gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation#Quaternion_from_axis-angle
  QuaternionTemplate<T> q;
  q.w = std::cos(radians / 2);
  q.x = q.y = q.z = std::sin(radians / 2);
  q.x *= axis.getX();
  q.y *= axis.getY();
  q.z *= axis.getZ();
  return q;
}

template <typename T>
inline void QuaternionTemplate<T>::Normalized() {
  *this = scaled(1 / std::sqrt(dot(*this)));
}

template <typename T>
inline void QuaternionTemplate<T>::Rotate(const QuaternionTemplate<T>& q2) {
  QuaternionTemplate<T> q;
  QuaternionTemplate<T>& q1 = *this;

  q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
  q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
  q.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
  q.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;

  q.Normalized();
  *this = q;
}

template<typename T>
QuaternionTemplate<T> QuaternionTemplate<T>::Conjugation() const {
  QuaternionTemplate<T> q = *this;
  q.x = -q.x;
  q.y = -q.y;
  q.z = -q.z;
  return q;
}

template<typename T>
QuaternionTemplate<T> QuaternionTemplate<T>::Inverse() const {
  QuaternionTemplate<T> q = Conjugation();
  q.Normalized();
  return q;
}
}

#endif  // BASE_MATSU_QUATERNION_IMPL_H_
