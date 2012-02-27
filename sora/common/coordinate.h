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
#ifndef SROA_COORDINATE_H_
#define SROA_COORDINATE_H_

namespace sora {;
template<typename T>
class SphericalPoint;
template<typename T>
class Rectangular3Point;

template<typename T>
class SphericalPoint {
public:
  static SphericalPoint CreateRadianBase(T radius, T theta_rad, T pi_rad);
  static SphericalPoint CreateDegreeBase(T radius, T theta_deg, T pi_deg);
  static SphericalPoint Create(const Rectangular3Point<T> &o);

  //getter
  T radius() const;
  T theta_deg() const;
  T pi_deg() const;
  T theta_rad() const;
  T pi_rad() const;

  //setter
  void set_radius(T r);
  void set_theta_deg(T deg);
  void set_theta_rad(T rad);
  void set_pi_deg(T deg);
  void set_pi_rad(T rad);

private:
  ///아래의 값은 degree로 저장한다.
  ///c std math함수의 인자는 전부다 rad기준이지만 어차피 deg-rad변환은 간단하니까
  ///속도상 큰 문제가 없을것이라고 추정되서 읽기 쉬운 deg를 기준으로 했다.
  ///rad가 0.43rad같은게 뜨면 감이 전혀 안오니까 이쪽이 개발에는 더 나을것이라고 판단했다
  ///후에 rad-deg의 변환을 바꾸는게 좋을거같으면 외부인터페스는 적당히 두고 여기를 고쳐서 떄우자 
  T radius_;
  T theta_deg_;
  T pi_deg_;
};

template<typename T>
class Rectangular3Point {
public:
  Rectangular3Point(T x, T y, T z);
  static Rectangular3Point Create(const SphericalPoint<T> &o);

  T x() const { return x_; }
  T y() const { return y_; }
  T z() const { return z_; }
private:
  T x_;
  T y_;
  T z_;
};

template<typename T>
class SphericalCoordinateSystem {
public:
};

template<typename T>
class Rectangular3CoordinateSystem {
public:
  Rectangular3Point<T> Convert(const SphericalPoint<T> &target);
};
}

//impl
#include "coordinate.hpp"

#endif  // SROA_COORDINATE_H_