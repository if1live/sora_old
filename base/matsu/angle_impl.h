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
#ifndef BASE_MATSU_ANGLE_IMPL_H_
#define BASE_MATSU_ANGLE_IMPL_H_

#include "sora/macro.h"

namespace matsu {
template<typename T>
Angle<T>::Angle(T degree) {
  // degree = -180~180사이의 범위가 되로록 유지하기
  if (degree >= -180 && degree <= 180) {
    degree_ = degree;
  } else if (degree > 180) {
    while (!(degree >= -180 && degree <= 180)) {
      degree -= 360;
    }
    degree_ = degree;
  } else {
    while (!(degree >= -180 && degree <= 180)) {
      degree += 360;
    }
    degree_ = degree;
  }

  SR_ASSERT(degree_ >= -180 && degree_ <= 180);
}
template<typename T>
Angle<T>::~Angle() {
}

template<typename T>
Angle<T> Angle<T>::Degree(T degree) {
  return Angle(degree);
}
template<typename T>
Angle<T> Angle<T>::Radian(T radian) {
  return Angle(RadianToDegree(radian));
}

template<typename T>
T Angle<T>::DegreeToRadian(T degree) {
  return degree * kPi / 180;
}
template<typename T>
T Angle<T>::RadianToDegree(T radian) {
  return radian * 180 / kPi;
}

template<typename T>
T Angle<T>::GetDegree() {
  return degree_;
}
template<typename T>
T Angle<T>::GetRadian() {
  return DegreeToRadian(degree_);
}

template<typename T>
bool Angle<T>::operator==(const Angle<T> &o) {
  if (o.degree_ == degree_) {
    return true;
  } else {
    return false;
  }
}
template<typename T>
bool Angle<T>::operator!=(const Angle<T> &o) {
  return !(*this == o);
}
}
#endif  // BASE_MATSU_ANGLE_IMPL_H_
