#pragma once

#include "sora/macro.h"

//boost static assert를 써서 컴파일 타입에 인덱스를 보장함(2차원 벡터에서는 z얻기를 못한다든가하는거)
//를 사용할 경우, getter, setter의 자동생성시 뻑난다. 그냥 무한차원중에서 앞부분만 쓰는 느낌으로 하자
//#include <boost/static_assert.hpp

//template impl
namespace matsu
{
template<typename T, int D>
VectorTemplate<T,D>::VectorTemplate()
{
  memset(data, 0, sizeof(T) * D);
}
template<typename T, int D>
template<unsigned int N>
VectorTemplate<T,D>::VectorTemplate(const T (&data)[N])
{
  SetVector(data);
}
template<typename T, int D>
template<typename T2>
VectorTemplate<T,D>::VectorTemplate(const VectorTemplate<T2,D> &o)
{
  for (int i = 0 ; i < D ; i++) {
    data[i] = static_cast<T>(o.data[i]);
  }
}
template<typename T, int D>
template<int D2>
VectorTemplate<T,D>::VectorTemplate(const VectorTemplate<T,D2> &o)
{
  memset(data, 0, sizeof(T) * D);
  for(int i = 0 ; i < D && i < D2 ; i++) {
    data[i] = o.data[i];
  }
}
template<typename T, int D>
template<int D2>
VectorTemplate<T,D>::VectorTemplate(const VectorTemplate<T,D2> &o, T etc)
{
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
VectorTemplate<T,D>::VectorTemplate(T x, T y)
{
  SetVector(x, y);
}
template<typename T, int D>
VectorTemplate<T,D>::VectorTemplate(T x, T y, T z)
{
  SetVector(x, y, z);
}
template<typename T, int D>
VectorTemplate<T,D>::VectorTemplate(T x, T y, T z, T w)
{
  SetVector(x, y, z, w);
}
template<typename T, int D>
void VectorTemplate<T,D>::SetVector(T x, T y)
{
  memset(data, 0, sizeof(T) * D);
  if (IsValidIndex(0))
  {
    data[0] = x;
  }
  if (IsValidIndex(1)) {
    data[1] = y;
  }
}
template<typename T, int D>
void VectorTemplate<T,D>::SetVector(T x, T y, T z)
{
  memset(data, 0, sizeof(T) * D);
  if (IsValidIndex(0)) {
    data[0] = x;
  }
  if (IsValidIndex(1)){
    data[1] = y;
  }
  if (IsValidIndex(2)) {
    data[2] = z;
  }
}
template<typename T, int D>
void VectorTemplate<T,D>::SetVector(T x, T y, T z, T w)
{
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
VectorTemplate<T,D>::~VectorTemplate()
{
}

template<typename T, int D>
template<unsigned int N>
void VectorTemplate<T,D>::SetVector(const T (&input)[N])
{
  int i = 0;
  for (i = 0 ; i < N && i < D ; i++) {
    data[i] = input[i];
  }
  //길이가 모자르고 남은부분은 0으로
  for ( ; i < D ; i++){
    data[i] = 0;
  }
}

template<typename T, int D>
VectorTemplate<T,D> VectorTemplate<T,D>::operator+(const VectorTemplate<T,D> &o) const
{
  VectorTemplate<T,D> tmp(*this);
  tmp += o;
  return tmp;
}

template<typename T, int D>
VectorTemplate<T,D>& VectorTemplate<T,D>::operator+=(const VectorTemplate<T,D> &o)
{
  for (int i = 0 ; i < D ; i++) {
    data[i] += o.data[i];
  }
  return *this;
}
template<typename T, int D>
VectorTemplate<T,D> VectorTemplate<T,D>::operator-(const VectorTemplate<T,D> &o) const
{
  VectorTemplate<T,D> tmp(*this);
  tmp -= o;
  return tmp;
}
template<typename T, int D>
VectorTemplate<T,D>& VectorTemplate<T,D>::operator-=(const VectorTemplate<T,D> &o)
{
  for (int i = 0 ; i < D ; i++)  {
    data[i] -= o.data[i];
  }
  return *this;
}
template<typename T, int D>
VectorTemplate<T,D> VectorTemplate<T,D>::operator-() const
{
  VectorTemplate<T,D> result;	//그냥 생성하면0
  result -= *this;
  return result;
}
template<typename T, int D>
VectorTemplate<T,D> VectorTemplate<T,D>::operator*(T t) const
{
  VectorTemplate<T,D> tmp(*this);
  tmp *= t;
  return tmp;
}
template<typename T, int D>
VectorTemplate<T,D>& VectorTemplate<T,D>::operator*=(T t)
{
  for (int i = 0 ; i < D ; i++)  {
    data[i] *= t;
  }
  return *this;
}
template<typename T, int D>
VectorTemplate<T,D> VectorTemplate<T,D>::operator/(T t) const
{
  VectorTemplate<T,D> tmp(*this);
  tmp /= t;
  return tmp;
}
template<typename T, int D>
VectorTemplate<T,D>& VectorTemplate<T,D>::operator/=(T t)
{
  for(int i = 0 ; i < D ; i++)  {
    data[i] /= t;
  }
  return *this;
}

template<typename T, int D>
T VectorTemplate<T,D>::Dot(const VectorTemplate<T,D> &o) const
{
  T result = 0;
  for (int i = 0 ; i < D ; i++) {
    result += data[i] * o.data[i];
  }
  return result;
}

template<typename T, int D>
VectorTemplate<T,3> VectorTemplate<T,D>::Cross(const VectorTemplate<T,3> &o) const
{
  SR_ASSERT(D == 3);
  T x = data[1] * o.data[2] - data[2] * o.data[1];
  T y = data[2] * o.data[0] - data[0] * o.data[2];
  T z = data[0] * o.data[1] - data[1] * o.data[0];
  return VectorTemplate<T,3>(x, y, z);
}

template<typename T, int D>
T VectorTemplate<T,D>::Size() const
{
  T size_square = SizeSquare();
  return (T)(::sqrtf((float)size_square));
}
template<typename T, int D>
T VectorTemplate<T,D>::SizeSquare() const
{
  T result = 0;
  for (int i = 0 ; i < D ; i++) {
    result += data[i] * data[i];
  }
  return result;
}

template<typename T, int D>
VectorTemplate<T,D> VectorTemplate<T,D>::Normalize() const
{
  VectorTemplate<T,D> tmp = *this;
  tmp.Normalized();
  return tmp;
}

template<typename T, int D>
void VectorTemplate<T,D>::Normalized()
{
  float s = 1.0f / Size();
  VectorTemplate<T,D> &tmp = *this;
  tmp *= s;
}

template<typename T, int D>
bool VectorTemplate<T,D>::IsZero(T errorRange) const
{
  T error = 1;
  for (int i = 0 ; i < D ; i++) {
    error *= errorRange;
  }
  return (error > SizeSquare());
}

template<typename T, int D>
bool VectorTemplate<T,D>::operator==(const VectorTemplate<T,D> &o) const
{
  for(int i = 0 ; i < D ; i++){
    if (data[i] != o.data[i]) {
      return false;
    }
  }
  return true;
}

template<typename T, int D>
bool VectorTemplate<T,D>::operator!=(const VectorTemplate<T,D> &o) const
{
  return !(*this == o);
}

template<typename T, int D>
int VectorTemplate<T,D>::GetDimension() const
{
  return D;
}

template<typename T, int D>
T* VectorTemplate<T,D>::Pointer()
{
  return data;
}
template<typename T, int D>
const T* VectorTemplate<T,D>::Pointer() const
{
  return data;
}

template<typename T, int D>
bool VectorTemplate<T,D>::IsValidIndex(int index) const
{
  return (D > index);
}
}