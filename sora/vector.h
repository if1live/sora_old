﻿/* Copyright (C) 2011 by if1live */
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
#ifndef SORA_VECTOR_H_
#define SORA_VECTOR_H_

namespace sora {;
template<typename T>  struct Vector2;
template<typename T>  struct Vector3;
template<typename T>  struct Vector4;
typedef Vector2<f32> vec2;
typedef Vector2<f32> ivec2;
typedef Vector3<f32> vec3;
typedef Vector3<i32> ivec3;
typedef Vector4<f32> vec4;
typedef Vector4<i32> ivec4;

template<typename T>
struct Vector2 {
  typedef T ElemType;
  enum {
    Dimension = 2,
  };
  union {
    struct {
      T x;
      T y;
    };
    T value[Dimension];
  };
  Vector2(T x, T y) : x(x), y(y) {}
  Vector2() : x(0), y(0) {}
  Vector2(const T(&data)[2]) : x(data[0]), y(data[1]) {}
};

template<typename T>
struct Vector3 {
  typedef T ElemType;
  enum {
    Dimension = 3,
  };
  union {
    struct {
      T x;
      T y;
      T z;
    };
    T value[Dimension];
  };
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
  Vector3() : x(0), y(0), z(0) {}
  Vector3(const T(&data)[3]) : x(data[0]), y(data[1]), z(data[2]) {}
};

template<typename T>
struct Vector4 {
  typedef T ElemType;
  enum {
    Dimension = 4,
  };
  union {
    struct {
      T x;
      T y;
      T z;
      T w;
    };
    T value[Dimension];
  };
  Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
  Vector4() : x(0), y(0), z(0), w(0) {}
  Vector4(const T(&data)[4]) : x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
};

template<typename VecType>
boolean operator==(const VecType &a, const VecType &b) {
  for (i32 i = 0 ; i < VecType::Dimension ; i++) {
    if (a.value[i] != b.value[i]) {
      return false;
    }
  }
  return true;
}
template<typename VecType>
boolean operator!=(const VecType &a, const VecType &b) {
  return !(a == b);
}
template<typename VecType>
VecType& operator+=(VecType &a, const VecType &b) {
  for (i32 i = 0 ; i < VecType::Dimension ; i++) {
    a.value[i] += b.value[i];
  }
  return a;
}
template<typename VecType>
VecType operator+(const VecType &a, const VecType &b) {
  VecType tmp = a;
  tmp += b;
  return tmp;
}
template<typename VecType>
VecType& operator-=(VecType &a, const VecType &b) {
  for (i32 i = 0 ; i < VecType::Dimension; i++) {
    a.value[i] -= b.value[i];
  }
  return a;
}
template<typename VecType>
VecType operator-(const VecType &a, const VecType &b) {
  VecType tmp = a;
  tmp -= b;
  return tmp;
}
template<typename VecType>
VecType& operator*=(VecType &a, typename VecType::ElemType t) {
  for (i32 i = 0 ; i < VecType::Dimension ; i++) {
    a.value[i] *= t;
  }
  return a;
}
template<typename VecType>
VecType operator*(const VecType &a, typename VecType::ElemType t) {
  VecType tmp = a;
  tmp *= t;
  return tmp;
}
template<typename VecType>
VecType& operator/=(VecType &a, typename VecType::ElemType t) {
  if (t == 0) {
    t = 1;
  }
  for (i32 i = 0 ; i < VecType::Dimension ; i++) {
    a.value[i] /= t;
  }
  return a;
}
template<typename VecType>
VecType operator/(const VecType &a, typename VecType::ElemType t) {
  VecType tmp = a;
  tmp /= t;
  return tmp;
}
template<typename VecType>
typename VecType::ElemType LengthSquare(const VecType &a) {
  typename VecType::ElemType retval = 0;
  for (i32 i = 0 ; i < VecType::Dimension ; i++) {
    retval += a.value[i] * a.value[i];
  }
  return retval;
}
template<typename VecType>
typename VecType::ElemType Length(const VecType &a) {
  typename VecType::ElemType length_square = LengthSquare(a);
  return sqrt(length_square);
}
template<typename VecType>
typename VecType::ElemType Dot(const VecType &a, const VecType &b) {
  typename VecType::ElemType retval = 0;
  for (i32 i = 0 ; i < VecType::Dimension ; i++) {
    retval += a.value[i] * b.value[i];
  }
  return retval;
}
template<typename VecType>
VecType &Normalized(VecType &a) {
  typename VecType::ElemType length = Length(a);
  return a /= length;
}
template<typename VecType>
VecType Normalize(const VecType &a) {
  VecType tmp = a;
  Normalized(tmp);
  return tmp;
}
template<typename T>
Vector3<T> Cross(const Vector3<T> &a, const Vector3<T> &b) {
  T x = a.y * b.z - a.z * b.y;
  T y = a.z * b.x - a.x * b.z;
  T z = a.x * b.y - a.y * b.x;
  return Vector3<T>(x, y, z);
}
}

#endif  // SORA_VECTOR_H_
