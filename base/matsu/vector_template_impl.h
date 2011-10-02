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
#ifndef BASE_MATSU_VECTOR_TEMPLATE_IMPL_H_
#define BASE_MATSU_VECTOR_TEMPLATE_IMPL_H_

#include "sora/macro.h"


namespace matsu {;
template<typename T, int D>
VectorTemplate<T, D>::VectorTemplate() {
  memset(data, 0, sizeof(T) * D);
}
template<typename T, int D>
template<unsigned int N>
VectorTemplate<T, D>::VectorTemplate(const T (&data)[N]) {
  SetVector(data);
}
template<typename T, int D>
template<typename T2>
VectorTemplate<T, D>::VectorTemplate(const VectorTemplate<T2, D> &o) {
  for (int i = 0 ; i < D ; i++) {
    data[i] = static_cast<T>(o.data[i]);
  }
}
template<typename T, int D>
template<int D2>
VectorTemplate<T, D>::VectorTemplate(const VectorTemplate<T, D2> &o) {
  memset(data, 0, sizeof(T) * D);
  for (int i = 0 ; i < D && i < D2 ; i++) {
    data[i] = o.data[i];
  }
}
template<typename T, int D>
template<int D2>
VectorTemplate<T, D>::VectorTemplate(const VectorTemplate<T, D2> &o, T etc) {
  memset(data, 0, sizeof(T) * dim);
  int i = 0;
  for (i = 0 ; i < D && i < D2 ; i++) {
    data[i] = o.data[i];
  }
  for ( ; i < D ; i++) {
    data[i] = etc;
  }
}
template<typename T, int D>
VectorTemplate<T, D>::VectorTemplate(T x, T y) {
  SetVector(x, y);
}
template<typename T, int D>
VectorTemplate<T, D>::VectorTemplate(T x, T y, T z) {
  SetVector(x, y, z);
}
template<typename T, int D>
VectorTemplate<T, D>::VectorTemplate(T x, T y, T z, T w) {
  SetVector(x, y, z, w);
}
template<typename T, int D>
void VectorTemplate<T, D>::SetVector(T x, T y) {
  memset(data, 0, sizeof(T) * D);
  if (IsValidIndex(0))   {
    data[0] = x;
  }
  if (IsValidIndex(1)) {
    data[1] = y;
  }
}
template<typename T, int D>
void VectorTemplate<T, D>::SetVector(T x, T y, T z) {
  memset(data, 0, sizeof(T) * D);
  if (IsValidIndex(0)) {
    data[0] = x;
  }
  if (IsValidIndex(1)) {
    data[1] = y;
  }
  if (IsValidIndex(2)) {
    data[2] = z;
  }
}
template<typename T, int D>
void VectorTemplate<T, D>::SetVector(T x, T y, T z, T w) {
  memset(data, 0, sizeof(T) * D);
  if (IsValidIndex(0)) {
    data[0] = x;
  }
  if (IsValidIndex(1)) {
    data[1] = y;
  }
  if (IsValidIndex(2)) {
    data[2] = z;
  }
  if (IsValidIndex(3)) {
    data[3] = w;
  }
}

template<typename T, int D>
VectorTemplate<T, D>::~VectorTemplate() {
}

template<typename T, int D>
template<unsigned int N>
void VectorTemplate<T, D>::SetVector(const T (&input)[N]) {
  int i = 0;
  for (i = 0 ; i < N && i < D ; i++) {
    data[i] = input[i];
  }
  // 길이가 모자르고 남은부분은 0으로
  for ( ; i < D ; i++) {
    data[i] = 0;
  }
}

template<typename T, int D>
VectorTemplate<T, D> VectorTemplate<T, D>::operator+(
  const VectorTemplate<T, D> &o) const {
  VectorTemplate<T, D> tmp(*this);
  tmp += o;
  return tmp;
}

template<typename T, int D>
VectorTemplate<T, D>& VectorTemplate<T, D>::operator+=(
  const VectorTemplate<T, D> &o) {
  for (int i = 0 ; i < D ; i++) {
    data[i] += o.data[i];
  }
  return *this;
}
template<typename T, int D>
VectorTemplate<T, D> VectorTemplate<T, D>::operator-(
  const VectorTemplate<T, D> &o) const {
  VectorTemplate<T, D> tmp(*this);
  tmp -= o;
  return tmp;
}
template<typename T, int D>
VectorTemplate<T, D>& VectorTemplate<T, D>::operator-=(
  const VectorTemplate<T, D> &o) {
  for (int i = 0 ; i < D ; i++)  {
    data[i] -= o.data[i];
  }
  return *this;
}
template<typename T, int D>
VectorTemplate<T, D> VectorTemplate<T, D>::operator-() const {
  VectorTemplate<T, D>  result;   // 그냥 생성하면0
  result -= *this;
  return result;
}
template<typename T, int D>
VectorTemplate<T, D> VectorTemplate<T, D>::operator*(T t) const {
  VectorTemplate<T, D> tmp(*this);
  tmp *= t;
  return tmp;
}
template<typename T, int D>
VectorTemplate<T, D>& VectorTemplate<T, D>::operator*=(T t) {
  for (int i = 0 ; i < D ; i++)  {
    data[i] *= t;
  }
  return *this;
}
template<typename T, int D>
VectorTemplate<T, D> VectorTemplate<T, D>::operator/(T t) const {
  VectorTemplate<T, D> tmp(*this);
  tmp /= t;
  return tmp;
}
template<typename T, int D>
VectorTemplate<T, D>& VectorTemplate<T, D>::operator/=(T t) {
  for (int i = 0 ; i < D ; i++)  {
    data[i] /= t;
  }
  return *this;
}

template<typename T, int D>
T VectorTemplate<T, D>::Dot(const VectorTemplate<T, D> &o) const {
  T result = 0;
  for (int i = 0 ; i < D ; i++) {
    result += data[i] * o.data[i];
  }
  return result;
}

template<typename T, int D>
VectorTemplate<T, 3> VectorTemplate<T, D>::Cross(
  const VectorTemplate<T, 3> &o) const {
  SR_ASSERT(D == 3);
  T x = data[1] * o.data[2] - data[2] * o.data[1];
  T y = data[2] * o.data[0] - data[0] * o.data[2];
  T z = data[0] * o.data[1] - data[1] * o.data[0];
  return VectorTemplate<T, 3>(x, y, z);
}

template<typename T, int D>
T VectorTemplate<T, D>::Size() const {
  T size_square = SizeSquare();
  return (T)(::sqrtf(static_cast<float>(size_square)));
}
template<typename T, int D>
T VectorTemplate<T, D>::SizeSquare() const {
  T result = 0;
  for (int i = 0 ; i < D ; i++) {
    result += data[i] * data[i];
  }
  return result;
}

template<typename T, int D>
VectorTemplate<T, D> VectorTemplate<T, D>::Normalize() const {
  VectorTemplate<T, D> tmp = *this;
  tmp.Normalized();
  return tmp;
}

template<typename T, int D>
void VectorTemplate<T, D>::Normalized() {
  float s = 1.0f / Size();
  VectorTemplate<T, D> &tmp = *this;
  tmp *= s;
}

template<typename T, int D>
bool VectorTemplate<T, D>::IsZero(T errorRange) const {
  T error = 1;
  for (int i = 0 ; i < D ; i++) {
    error *= errorRange;
  }
  return (error > SizeSquare());
}

template<typename T, int D>
bool VectorTemplate<T, D>::operator==(const VectorTemplate<T, D> &o) const {
  for (int i = 0 ; i < D ; i++) {
    if (data[i] != o.data[i]) {
      return false;
    }
  }
  return true;
}

template<typename T, int D>
bool VectorTemplate<T, D>::operator!=(const VectorTemplate<T, D> &o) const {
  return !(*this == o);
}

template<typename T, int D>
int VectorTemplate<T, D>::GetDimension() const {
  return D;
}

template<typename T, int D>
T* VectorTemplate<T, D>::Pointer() {
  return data;
}
template<typename T, int D>
const T* VectorTemplate<T, D>::Pointer() const {
  return data;
}

template<typename T, int D>
bool VectorTemplate<T, D>::IsValidIndex(int index) const {
  return (D > index);
}
}

#endif  // BASE_MATSU_VECTOR_TEMPLATE_IMPL_H_
