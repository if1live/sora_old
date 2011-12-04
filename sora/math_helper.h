// Copyright (C) 2011 by if1live
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
#ifndef SORA_MATH_HELPER_H_
#define SORA_MATH_HELPER_H_

#if SR_USE_PCH == 0
#include <cmath>
#endif

#include "sora/vector.h"
#include "sora/matrix.h"

namespace sora {;
const f32 kPi = 3.141592f;
const f32 kPiOver2 = kPi / 2;
const f32 kPiOver4 = kPi / 4;

inline f32 DegreeToRadian(f32 degree) {
  return degree * kPi / 180.0f;
}
inline f32 RadianToDegree(f32 radian) {
  return radian * 180.0f / kPi;
}

template<typename T, i32 D>
void SetIdentity(Matrix<T, D, D> *m) {
  memset(m->value, 0, sizeof(T) * D * D);
  for (i32 i = 0 ; i < D ; i++) {
    m->Set(i, i, 1);
  }
}

template<typename T, i32 D>
boolean IsIdentity(const Matrix<T, D, D> &m) {
  for (i32 x = 0 ; x < D ; x++) {
    for (i32 y = 0 ; y < D ; y++) {
      T elem = m.Get(x, y);
      if (x == y) {
        if (elem != 1) {
          return false;
        }
      } else {
        if (elem != 0) {
          return false;
        }
      }
    }
  }
  return true;
}

template<typename T>
void SetTranslate(T x, T y, T z, Matrix<T, 4, 4> *m) {
 T data[] = {
   1, 0, 0, 0,
   0, 1, 0, 0,
   0, 0, 1, 0,
   x, y, z, 1
  };
 m->Set(data);
}

template<typename T>
void SetScale(T x, T y, T z, Matrix<T, 4, 4> *m) {
  T data[] = {
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1
  };
  m->Set(data);
}
template<typename T>
void SetScale(T s, Matrix<T, 4, 4> *m) {
  SetScale(s, s, s, m);
}

template <typename T>
void SetRotateX(T degrees, Matrix<T, 4, 4> *m) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
      1, 0, 0, 0,
      0, c, s, 0,
      0, -s, c, 0,
      0, 0, 0, 1
  };
  m->Set(data);
}
template <typename T>
void SetRotateY(T degrees, Matrix<T, 4, 4> *m) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
      c, 0, -s, 0,
      0, 1, 0, 0,
      s, 0, c, 0,
      0, 0, 0, 1,
  };
  m->Set(data);
}
template <typename T>
void SetRotateZ(T degrees, Matrix<T, 4, 4> *m) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
      c, s, 0, 0,
      -s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  };
  m->Set(data);
}

mat4 &SetOrtho(float left, float right, float bottom, float top, float near_val, float far_val, mat4 *m);
mat4 &SetFrustum(float left, float right, float bottom, float top, float near_val, float far_val, mat4 *m);
mat4 &SetLookAt(float eye_x, float eye_y, float eye_z,
  float target_x, float target_y, float target_z,
  float up_x, float up_y, float up_z, mat4 *m);
mat4 &SetPerspective(float fovy, float aspect, float zNear, float zFar, mat4 *m);
}

#endif  // SORA_MATH_HELPER_H_
