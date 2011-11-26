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

namespace sora {;
template<typename T, int R, int C>  struct Matrix;
typedef Matrix<int, 2, 2> imat2;
typedef Matrix<int, 3, 3> imat3;
typedef Matrix<int, 4, 4> imat4;
typedef Matrix<float, 2, 2> mat2;
typedef Matrix<float, 3, 3> mat3;
typedef Matrix<float, 4, 4> mat4;

template<typename T, int R, int C>
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
  template<unsigned int N>
  Matrix(const T(&data)[N]) {
    Set(data);
  }
  template<int R2, int C2>
  Matrix(const Matrix<T, R2, C2> &o) {
    memset(value, 0, sizeof(T) * R * C);
    for (int i = 0 ; i < R2 && i < R ; i++) {
      for (int j = 0 ; j < C && j < C2 ; j++) {
        Set(i, j, o.Get(i, j));
      }
    }
  }
  template<typename T2>
  Matrix(const Matrix<T2, R, C> &o) {
    for (int i = 0 ; i < R * C; i++) {
      value[i] = static_cast<T> (o.value[i]);
    }
  }
  ~Matrix() { }

  T Get(int r, int c) const {
    return value[GetIndex(r, c)];
  }
  void Set(int r, int c, T value) {
    this->value[GetIndex(r, c)] = value;
  }
  template<unsigned int N>
  void Set(const T(&data)[N]) {
    /// @TODO check size?
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
    for (int i = 0 ; i < R*C ; i++) {
      value[i] += o.value[i];
    }
  }
  Matrix<T, R, C> operator-(const Matrix<T, R, C> &o) const {
    Matrix<T, R, C> tmp = *this;
    tmp -= o;
    return tmp;
  }
  Matrix<T, R, C> &operator-=(const Matrix<T, R, C> &o) {
    for (int i = 0 ; i < R*C ; i++) {
      value[i] -= o.value[i];
    }
  }
  /// @brief *=는 자신의 값 자체가 바뀌므로 행렬이 정행렬일때만 가능하다
  template<int S>
  Matrix<T, S, S> &operator*=(const Matrix<T, S, S> &o) {
    Matrix<T, S, S> result;
    for (int i = 0 ; i < S ; i++) {
      for (int j = 0 ; j < S ; j++) {
        T row_data[S];
        T col_data[S];
        GetRow(i, row_data);
        GetCol(j, col_data);

        T dot_value = 0;
        for (int x = 0 ; x < S ; x++) {
          dot_value += row_data[x] * col_data[x];
        }
        result.Set(i, j, dot_value);
      }
    }
    memcpy(value, result.value, sizeof(T) * R * C);
    return *this;
  }

  template<int C2>
  Matrix<T, R, C2> operator*(const Matrix<T, C, C2> &o) const {
    Matrix<T, R, C2> result;
    for (int i = 0 ; i < R ; i++) {
      for (int j = 0 ; j < C2 ; j++) {

        VectorTemplate<T, C> rowVec = GetRow(i);
        VectorTemplate<T, C> colVec = o.GetCol(j);
        T value = rowVec.Dot(colVec);
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
    for (int i = 0 ; i < R*C ; i++) {
      value[i] *= s;
    }
  }

  //VectorTemplate<T, row> operator*(const VectorTemplate<T, col> &o) const;

  bool operator==(const Matrix<T, R, C> &o) const {
    if (memcmp(value, o.value, sizeof(T) * R * C) == 0) {
      return true;
    } else {
      return false;
    }
  }
  bool operator!=(const Matrix<T, R, C> &o) const {
    return !(*this == o);
  }

  void GetRow(int r, T retval[C]) const {
    for (int i = 0 ; i < C ; i++) {
      retval[i] = Get(r, i);
    }
  }
  void GetCol(int c, T retval[R]) const {
    for (int i = 0 ; i < R ; i++) {
      retval[i] = Get(i, c);
    }
  }
  void SetRow(int r, T data[C]) {
    for (int i = 0 ; i < C ; i++) {
      Set(r, i, data[i]);
    }
  }
  void SetCol(int c, T data[R]) {
    for (int i = 0 ; i < R ; i++) {
      Set(i, c, data[i]);
    }
  }
  
  Matrix<T, C, R> Transpose() const {
    Matrix<T, C, R> result;
    for (int i = 0 ; i < R ; i++) {
      for (int j = 0 ; j < C ; j++) {
        result.Set(j, i, Get(i, j));
      }
    }
    return result;
  }


  int GetIndex(int r, int c) const {
    int index = c*R + r;
    return index;
  }
};

}
#endif  // SORA_MATRIX_H_
