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
#pragma once
#ifndef BASE_MATSU_COORDINATE_H_
#define BASE_MATSU_COORDINATE_H_

#include "sora/macro.h"

namespace matsu {;
template<typename T>
class SphericalPoint;
template<typename T>
class Rectangular3Point;

template<typename T>
class SphericalPoint {
 public:
  static SphericalPoint CreateRadianBase(T radius, T theta_radian, T pi_radian);
  static SphericalPoint CreateDegreeBase(T radius, T theta_degree, T pi_degree);
  static SphericalPoint Create(const Rectangular3Point<T> &o);

  /// 아래의 값은 degree로 저장한다.
  /// c std math함수의 인자는 전부다 rad기준이지만
  /// 어차피 deg-rad변환은 간단하니까
  /// 속도상 큰 문제가 없을것이라고 추정되서 읽기 쉬운 deg를 기준으로 했다.
  /// rad가 0.43rad같은게 뜨면 감이 전혀 안오니까
  /// 이쪽이 개발에는 더 나을것이라고 판단했다
  /// 후에 radian-deggree의 변환을 바꾸는게
  /// 좋을거같으면 외부인터페스는 적당히 두고
  /// 여기를 고쳐서 때우면 될듯
  SR_GETTER_SETTER(T, radius);
  SR_GETTER_SETTER(T, theta_degree);
  SR_GETTER_SETTER(T, pi_degree);
};

template<typename T>
class Rectangular3Point {
 public:
  Rectangular3Point(T x, T y, T z);
  static Rectangular3Point Create(const SphericalPoint<T> &o);
 public:
  T x;
  T y;
  T z;
};
}

#include "matsu/coordinate_impl.h"

#endif  // BASE_MATSU_COORDINATE_H_
