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
#ifndef SORA_CORE_VECTOR_H_
#define SORA_CORE_VECTOR_H_

#if SR_USE_PCH == 0
#include "arch.h"
#endif

//#ifdef __cplusplus
//extern "C" {;
//#endif

namespace sora {;
typedef float vec2f_t[2];
typedef float vec3f_t[3];
typedef float vec4f_t[4];
typedef vec2f_t vec2_t;
typedef vec3f_t vec3_t;
typedef vec4f_t vec4_t;

template<typename T> struct Vector2;
template<typename T> struct Vector3;
template<typename T> struct Vector4;
typedef Vector2<float> Vec2f;
typedef Vector3<float> Vec3f;
typedef Vector4<float> Vec4f;
typedef Vector2<float> Vec2;
typedef Vector3<float> Vec3;
typedef Vector4<float> Vec4;
typedef Vector2<ushort> Vec2us;
typedef Vector3<ushort> Vec3us;
typedef Vector4<ushort> Vec4us;

template<typename T>
struct Vector2 {
  typedef T ElemType;
  enum { Dim = 2 };
  //data
  union {
    T data[2];
    struct { T x, y; };
  };

  Vector2() : x(0), y(0) {}
  Vector2(T x, T y) : x(x), y(y) {}
  Vector2(T (&v)[2]) { memcpy(data, v, sizeof(T) * Dim); }
  Vector2&  operator+=(const Vector2 &o) {
    VecAdd(data, o.data, data);
    return *this;
  }
  Vector2   operator+(const Vector2 &o) const {
    Vector2 tmp = *this;
    tmp += o;
    return tmp;
  }
  Vector2&  operator-=(const Vector2 &o) {
    VecSub(data, o.data, data);
    return *this;
  }
  Vector2   operator-(const Vector2 &o) const {
    Vector2 tmp = *this;
    tmp -= o;
    return tmp;
  }
  Vector2&  operator*=(T o) {
    VecMul(data, o, data);
    return *this;
  }
  Vector2   operator*(T o) const {
    Vector2 tmp = *this;
    tmp *= o;
    return tmp;
  }
  Vector2&  operator/=(T o) {
    VecDiv(data, o, data);
    return *this;
  }
  Vector2   operator/(T o) const {
    Vector2 tmp = *this;
    tmp /= o;
    return tmp;
  }
  Vector2   operator-() const {
    Vector2 tmp = *this;
    tmp *= -1;
    return tmp;
  }
  T LengthSquare() const {
    return VecLengthSquare(data);
  }
  T Length() const {
    return VecLength(data);
  }
  void Normalized() {
    VecNormalized(data);
  }
  Vector2 Normalize() const {
    Vector2 tmp = *this;
    tmp.Normalized();
    return tmp;
  }
  T Dot(const Vector2 &o) const {
    return VecDot(data, o.data);
  }
  bool operator==(const Vector2 &o) const {
    return (memcmp(data, o.data, sizeof(T) * Dim) == 0);
  }
  bool operator!=(const Vector2 &o) const {
    return !(*this == o);
  }
};

template<typename T>
struct Vector3 {
  enum { Dim = 3 };
  //data
  union {
    T data[3];
    struct { T x, y, z; };
  };

  //operator 
  Vector3() : x(0), y(0), z(0) {}
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
  Vector3(T (&v)[3]) { memcpy(data, v, sizeof(T) * Dim); }
  Vector3&  operator+=(const Vector3 &o) {
    VecAdd(data, o.data, data);
    return *this;
  }
  Vector3   operator+(const Vector3 &o) const {
    Vector3 tmp = *this;
    tmp += o;
    return tmp;
  }
  Vector3&  operator-=(const Vector3 &o) {
    VecSub(data, o.data, data);
    return *this;
  }
  Vector3   operator-(const Vector3 &o) const {
    Vector3 tmp = *this;
    tmp -= o;
    return tmp;
  }
  Vector3&  operator*=(T o) {
    VecMul(data, o, data);
    return *this;
  }
  Vector3   operator*(T o) const {
    Vector3 tmp = *this;
    tmp *= o;
    return tmp;
  }
  Vector3&  operator/=(T o) {
    VecDiv(data, o, data);
    return *this;
  }
  Vector3   operator/(T o) const {
    Vector3 tmp = *this;
    tmp /= o;
    return tmp;
  }
  Vector3   operator-() const {
    Vector3 tmp = *this;
    tmp *= -1;
    return tmp;
  }
  T LengthSquare() const {
    return VecLengthSquare(data);
  }
  T Length() const {
    return VecLength(data);
  }
  void Normalized() {
    VecNormalized(data);
  }
  Vector3 Normalize() const {
    Vector3 tmp = *this;
    tmp.Normalized();
    return tmp;
  }
  T Dot(const Vector3 &o) const {
    return VecDot(data, o.data);
  }
  bool operator==(const Vector3 &o) const {
    return (memcmp(data, o.data, sizeof(T) * Dim) == 0);
  }
  bool operator!=(const Vector3 &o) const {
    return !(*this == o);
  }
};

template<typename T>
struct Vector4 {
  enum { Dim = 4 };
  //data
  union {
    T data[4];
    struct { T x, y, z, w; };
  };

  //operator 
  Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
  Vector4(T (&v)[4]) { memcpy(data, v, sizeof(T) * Dim); }
  Vector4&  operator+=(const Vector4 &o) {
    VecAdd(data, o.data, data);
    return *this;
  }
  Vector4   operator+(const Vector4 &o) const {
    Vector4 tmp = *this;
    tmp += o;
    return tmp;
  }
  Vector4&  operator-=(const Vector4 &o) {
    VecSub(data, o.data, data);
    return *this;
  }
  Vector4   operator-(const Vector4 &o) const {
    Vector4 tmp = *this;
    tmp -= o;
    return tmp;
  }
  Vector4&  operator*=(T o) {
    VecMul(data, o, data);
    return *this;
  }
  Vector4   operator*(T o) const {
    Vector4 tmp = *this;
    tmp *= o;
    return tmp;
  }
  Vector4&  operator/=(T o) {
    VecDiv(data, o, data);
    return *this;
  }
  Vector4   operator/(T o) const {
    Vector4 tmp = *this;
    tmp /= o;
    return tmp;
  }
  Vector4   operator-() const {
    Vector4 tmp = *this;
    tmp *= -1;
    return tmp;
  }
  T LengthSquare() const {
    return VecLengthSquare(data);
  }
  T Length() const {
    return VecLength(data);
  }
  void Normalized() {
    VecNormalized(data);
  }
  Vector4 Normalize() const {
    Vector4 tmp = *this;
    tmp.Normalized();
    return tmp;
  }
  T Dot(const Vector4 &o) const {
    return VecDot(data, o.data);
  }
  bool operator==(const Vector4 &o) const {
    return (memcmp(data, o.data, sizeof(T) * Dim) == 0);
  }
  bool operator!=(const Vector4 &o) const {
    return !(*this == o);
  }
};

template<typename T, int D>
void VecSetZero(T (&a)[D]) {
  memset(&a, 0, sizeof(T) * D);
}

template<typename T, int D>
void VecSet(float x, float y, T (&v)[D]) {
  v[0] = x;
  v[1] = y;
}

template<typename T, int D>
void VecSet(float x, float y, float z, T (&v)[D]) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
}

template<typename T, int D>
void VecSet(float x, float y, float z, float w, T (&v)[D]) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = w;
}

template<typename T, int D>
void VecCopy(const T (&src)[D], T (&dst)[D]) {
  memcpy(&dst, &src, sizeof(T) * D);
}

template<typename T, int D>
void VecAdd(const T (&a)[D], const T (&b)[D], T (&c)[D]) {
  for(int i = 0 ; i < D ; i++) {
    c[i] = a[i] + b[i];
  }
}

template<typename T, int D>
void VecSub(const T (&a)[D], const T (&b)[D], T (&c)[D]) {
  for(int i = 0 ; i < D ; i++) {
    c[i] = a[i] - b[i];
  }
}

template<typename T, int D>
T VecDot(const T (&a)[D], const T (&b)[D]) {
  T dot = 0;
  for(int i = 0 ; i < D ; i++) {
    dot += a[i] * b[i];
  }
  return dot;
}

template<typename T, int D>
void VecMul(const T (&a)[D], T b, T (&c)[D]) {
  for(int i = 0 ; i < D ; i++) { 
    c[i] = a[i] * b; 
  }
}

template<typename T, int D>
void VecDiv(const T (&a)[D], T b, T (&c)[D]) {
  for(int i = 0 ; i < D ; i++) { 
    c[i] = a[i] / b; 
  }
}

//0으로 나누기같은거 확인하는거
template<typename T, int D>
int VecDivCheck(const T (&a)[D], T b, T (&c)[D]) {
  if(b == 0) {
    return -1;
  }
  for(int i = 0 ; i < D ; i++) { 
    c[i] = a[i] / b; 
  }
  return 0;
}

template<typename T, int D>
bool VecEq(const T (&a)[D], const T (&b)[D]) {
  if(memcmp(&a, &b, sizeof(T) * D) == 0) {
    return true;
  } else {
    return false;
  }
}

template<typename T, int D>
bool VecNotEq(const T (&a)[D], const T (&b)[D]) {
  return !VecEq(a, b);
}

template<typename T, int D>
T VecLengthSquare(const T (&a)[D]) {
  T square = 0;
  for(int i = 0 ; i < D ; i++) {
    square += a[i] * a[i];
  }
  return square;
}

template<typename T, int D>
T VecLength(const T (&a)[D]) {
  T length_square = VecLengthSquare(a);
  T value = sqrt(length_square);
  return value;
}

template<typename T, int D>
void VecNormalized(T (&a)[D]) {
  T length = VecLength(a);
  return VecDiv(a, length, a);
}

template<typename T, int D>
void VecNormalize(const T (&src)[D], T (&dst)[D]) {
  memcpy(&dst, &src, sizeof(T) * D);
  VecNormalized(dst);
}

template<typename T>
void VecCross(const T (&a)[3], const T (&b)[3], T (&dst)[3]) {
  T x = a[1] * b[2] - a[2] * b[1];
  T y = a[2] * b[0] - a[0] * b[2];
  T z = a[0] * b[1] - a[1] * b[0];
  dst[0] = x; dst[1] = y; dst[2] = z;
}

}

/*
template<typename VecType>
bool VectorIsZero(const VecType &a, typename VecType::ElemType error) {
  typename VecType::ElemType length_square = VectorLengthSquare(a);
  if (length_square > error) {
    return false;
  }
  return true;
}
*/

//int func1();
//int func2();

namespace sora {;
SR_C_DLL float Vec2f_testFunc(float x, float y);
}
//#ifdef __cplusplus
//}
//#endif

#endif  // SORA_CORE_VECTOR_H_