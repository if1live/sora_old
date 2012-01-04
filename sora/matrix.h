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
#ifndef SORA_MATRIX_H_
#define SORA_MATRIX_H_

#if SR_USE_PCH == 0
#include <cstring>
#endif
#include "sora/vector.h"

namespace sora {;

typedef int i32;
typedef unsigned int u32;

template<typename T, i32 R, i32 C>  struct Matrix;
typedef Matrix<int, 2, 2> imat2;
typedef Matrix<int, 3, 3> imat3;
typedef Matrix<int, 4, 4> imat4;
typedef Matrix<float, 2, 2> mat2;
typedef Matrix<float, 3, 3> mat3;
typedef Matrix<float, 4, 4> mat4;

template<typename T, i32 R, i32 C>
struct Matrix {
  typedef T ElemType;
  enum {
    Row = R,
    Col = C,
  };
  T value[R*C];

 public:
  Matrix() {
    memset(value, 0, sizeof(T) * R * C);
  }
  template<u32 N>
  Matrix(const T(&data)[N]) {
    Set(data);
  }
  Matrix(const T *data) {
    Set(data);
  }
  template<i32 R2, i32 C2>
  Matrix(const Matrix<T, R2, C2> &o) {
    memset(value, 0, sizeof(T) * R * C);
    for (i32 i = 0 ; i < R2 && i < R ; i++) {
      for (i32 j = 0 ; j < C && j < C2 ; j++) {
        Set(i, j, o.Get(i, j));
      }
    }
  }
  template<typename T2>
  Matrix(const Matrix<T2, R, C> &o) {
    for (i32 i = 0 ; i < R * C; i++) {
      value[i] = static_cast<T> (o.value[i]);
    }
  }
  ~Matrix() { }

  T Get(i32 r, i32 c) const {
    return value[GetIndex(r, c)];
  }
  void Set(i32 r, i32 c, T value) {
    this->value[GetIndex(r, c)] = value;
  }
  template<u32 N>
  void Set(const T(&data)[N]) {
    BOOST_STATIC_ASSERT(N == R * C);
    memcpy(value, data, sizeof(T) * R * C);
  }
  void Set(const T *data) {
    memcpy(value, data, sizeof(T) * R * C);
  }

  // operator
  Matrix<T, R, C> operator+(const Matrix<T, R, C> &o) const {
    Matrix<T, R, C> tmp = *this;
    tmp += o;
    return tmp;
  }
  Matrix<T, R, C> &operator+=(const Matrix<T, R, C> &o)  {
    for (i32 i = 0 ; i < R*C ; i++) {
      value[i] += o.value[i];
    }
  }
  Matrix<T, R, C> operator-(const Matrix<T, R, C> &o) const {
    Matrix<T, R, C> tmp = *this;
    tmp -= o;
    return tmp;
  }
  Matrix<T, R, C> &operator-=(const Matrix<T, R, C> &o) {
    for (i32 i = 0 ; i < R*C ; i++) {
      value[i] -= o.value[i];
    }
  }
  /// @brief *=는 자신의 값 자체가 바뀌므로 행렬이 정행렬일때만 가능하다
  template<i32 S>
  Matrix<T, S, S> &operator*=(const Matrix<T, S, S> &o) {
    Matrix<T, S, S> result;
    for (i32 i = 0 ; i < S ; i++) {
      for (i32 j = 0 ; j < S ; j++) {
        T row_data[S];
        T col_data[S];
        GetRow(i, row_data);
        o.GetCol(j, col_data);

        T dot_value = 0;
        for (i32 x = 0 ; x < S ; x++) {
          dot_value += row_data[x] * col_data[x];
        }
        result.Set(i, j, dot_value);
      }
    }
    memcpy(value, result.value, sizeof(T) * R * C);
    return *this;
  }

  template<i32 C2>
  Matrix<T, R, C2> operator*(const Matrix<T, C, C2> &o) const {
    Matrix<T, R, C2> result;
    for (i32 i = 0 ; i < R ; i++) {
      for (i32 j = 0 ; j < C2 ; j++) {

        T row_data[C];
        T col_data[C];
        GetRow(i, row_data);
        o.GetCol(j, col_data);
        
        T value = 0;
        for (int i = 0 ; i < C ; i++) {
          value += row_data[i] * col_data[i];
        }
        result.Set(i, j, value);
      }
    }
    return result;
  }

  Matrix<T, R, C> operator*(T s) const {
    Matrix<T, R, C> tmp = *this;
    tmp *= s;
    return tmp;
  }
  Matrix<T, R, C> &operator*=(T s) {
    for (i32 i = 0 ; i < R*C ; i++) {
      value[i] *= s;
    }
    return *this;
  }

  Vector4<T> operator*(const Vector4<T> &b) const {
    Vector4<T> v;
    v.x = Get(0, 0)*b.x + Get(1, 0)*b.y + Get(2, 0)*b.z + Get(3, 0)*b.w;
		v.y = Get(0, 1)*b.x + Get(1, 1)*b.y + Get(2, 1)*b.z + Get(3, 1)*b.w;
		v.z = Get(0, 2)*b.x + Get(1, 2)*b.y + Get(2, 2)*b.z + Get(3, 2)*b.w;
		v.w = Get(0, 3)*b.x + Get(1, 3)*b.y + Get(2, 3)*b.z + Get(3, 3)*b.w;
		return v;
  }

  boolean operator==(const Matrix<T, R, C> &o) const {
    if (memcmp(value, o.value, sizeof(T) * R * C) == 0) {
      return true;
    } else {
      return false;
    }
  }
  boolean operator!=(const Matrix<T, R, C> &o) const {
    return !(*this == o);
  }

  void GetRow(i32 r, T retval[C]) const {
    for (i32 i = 0 ; i < C ; i++) {
      retval[i] = Get(r, i);
    }
  }
  void GetCol(i32 c, T retval[R]) const {
    for (i32 i = 0 ; i < R ; i++) {
      retval[i] = Get(i, c);
    }
  }
  void SetRow(i32 r, T data[C]) {
    for (i32 i = 0 ; i < C ; i++) {
      Set(r, i, data[i]);
    }
  }
  void SetCol(i32 c, T data[R]) {
    for (i32 i = 0 ; i < R ; i++) {
      Set(i, c, data[i]);
    }
  }
  
  Matrix<T, C, R> Transpose() const {
    Matrix<T, C, R> result;
    for (i32 i = 0 ; i < R ; i++) {
      for (i32 j = 0 ; j < C ; j++) {
        result.Set(j, i, Get(i, j));
      }
    }
    return result;
  }


  i32 GetIndex(i32 r, i32 c) const {
    i32 index = c*R + r;
    return index;
  }
};

}
#endif  // SORA_MATRIX_H_
