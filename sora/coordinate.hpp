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

#include "math_helper.h"

namespace sora {;

template<typename T>
SphericalPoint<T> SphericalPoint<T>::CreateRadianBase(T radius, T theta_rad, T pi_rad) {
  T tmp_thera_deg = RadianToDegree(theta_rad);
  T tmp_pi_deg = RadianToDegree(pi_rad);
  return CreateDegreeBase(radius, tmp_thera_deg, tmp_pi_deg);
}

template<typename T>
SphericalPoint<T> SphericalPoint<T>::CreateDegreeBase(T radius, T theta_deg, T pi_deg) {
  SphericalPoint p;
  //radius, theta, pi의 값이 적절한 범위에 존재하는지 확인하고 아니면 적절한값으로 변환 
  p.set_radius(radius);
  p.set_theta_deg(theta_deg);
  p.set_pi_deg(pi_deg);
  return p;
}

template<typename T>
SphericalPoint<T> SphericalPoint<T>::Create(const Rectangular3Point<T> &o) {
  T x = o.x();
  T y = o.y();
  T z = o.z();

  T r = sqrt(x*x + y*y + z*z);
  if(r == 0) {
    //원점인 경우,
    return CreateDegreeBase(0, 0, 0);
  }
  //c std math func의 반환이니까 rad로 나온다 
  T theta = acos(z/r);

  T pi = 0;
  if(x != 0) {
    //x가 0이면 y/x를 계산할수 없다. 
    pi = atan(y/x);
  } else {
    //x가 -이라는것은 pi의 각도는 90이거나 270이라는것이다. 이것은 y의 부호에 따라 결정된다 
    if(y > 0) {
      pi = DegreeToRadian(static_cast<T>(90.0f));
    } else if(y == 0) {
      pi = DegreeToRadian(static_cast<T>(0.0f));
    } else {
      pi = DegreeToRadian(static_cast<T>(270.0f));
    }
  }
  return CreateRadianBase(r, theta, pi);
}

template<typename T>
void SphericalPoint<T>::set_radius(T r) {
  //radius는 0보다 크거나 같도록 만들기 
  T radius = r > 0 ? r : -r;
  radius_ = radius;
}
template<typename T>
void SphericalPoint<T>::set_theta_deg(T deg) {
  //theta의 범위:0~180
  int thetaLoopCnt = deg / 180.0f;
  if(deg < 0) {
    //-10 / 180은 0이니까 1을 더해서 넘긴다 
    thetaLoopCnt -= 1;
  }
  theta_deg_ = deg - thetaLoopCnt * 180.0f;
  SR_ASSERT(theta_deg_ >= 0 && theta_deg_ <= 180.0f);
}
template<typename T>
void SphericalPoint<T>::set_theta_rad(T rad) {
  set_theta_deg(RadianToDegree(rad));
}
template<typename T>
void SphericalPoint<T>::set_pi_deg(T deg) {
  //pi의 범위:0~360 
  int piLoopCnt = deg / 360.0f;
  if(deg < 0) {
    piLoopCnt -= 1;
  }
  pi_deg_ = deg - piLoopCnt * 360.0f;
  SR_ASSERT(pi_deg_ >= 0 && pi_deg_ <= 360.0f);
}
template<typename T>
void SphericalPoint<T>::set_pi_rad(T rad) {
  set_pi_deg(RadianToDegree(rad));
}

template<typename T> 
T SphericalPoint<T>::theta_deg() const {
  return theta_deg_;
}
template<typename T>
T SphericalPoint<T>::pi_deg() const {
  return pi_deg_;
}
template<typename T>
T SphericalPoint<T>::theta_rad() const {
  return DegreeToRadian(theta_deg_);
}
template<typename T>
T SphericalPoint<T>::pi_rad() const {
  return DegreeToRadian(pi_deg_);
}
template<typename T>
T SphericalPoint<T>::radius() const {
  return radius_;
}


template<typename T>
Rectangular3Point<T>::Rectangular3Point(T x, T y, T z)
  : x_(x), y_(y), z_(z) { }
template<typename T>
Rectangular3Point<T> Rectangular3Point<T>::Create(const SphericalPoint<T> &o) {
  //TODO:...
  //구면극좌표를 변환하는 일은 아직 없다. 필요해지면 그떄 구현하자 
  SR_ASSERT(!"TODO: impl");
}

}
