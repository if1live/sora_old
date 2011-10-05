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
#ifndef BASE_MATSU_COORDINATE_IMPL_H_
#define BASE_MATSU_COORDINATE_IMPL_H_
#include "matsu/math.h"
#include "matsu/angle.h"

namespace matsu {
  template<typename T>
  SphericalPoint<T> SphericalPoint<T>::CreateRadianBase(T radius,
    T theta_radian, T pi_radian) {
    T theraDegree = Angle<T>::RadianToDegree(theta_radian);
    T pi_degree = Angle<T>::RadianToDegree(pi_radian);
    return CreateDegreeBase(radius, theraDegree, pi_degree);
  }

  template<typename T>
  SphericalPoint<T> SphericalPoint<T>::CreateDegreeBase(T radius,
    T theta_degree, T pi_degree) {
    SphericalPoint p;
    // radius, theta, pi의 값이 적절한 범위에 존재하는지
    // 확인하고 아니면 적절한값으로 변환
    if (radius < 0) {
      p.radius_ = -radius;
    } else {
      p.radius_ = radius;
    }

    // theta의 범위:0~180
    int theta_loop_count = static_cast<int>(theta_degree / 180.0f);
    if (theta_degree < 0) {
      // -10 / 180은 0이니까 1을 더해서 넘긴다
      theta_loop_count -= 1;
    }
    p.theta_degree_ = theta_degree - theta_loop_count * 180.0f;
    SR_ASSERT(p.theta_degree_ >= 0 && p.theta_degree_ <= 180.0f);

    Angle<T> pi = Angle<T>::Degree(pi_degree);
    p.pi_degree_ = pi.GetDegree();
    return p;
  }

  template<typename T>
  SphericalPoint<T> SphericalPoint<T>::Create(const Rectangular3Point<T> &o) {
    T x = o.x;
    T y = o.y;
    T z = o.z;

    T r = sqrt(x*x + y*y + z*z);
    if (r == 0) {
      // 원점인 경우,
      return CreateDegreeBase(0, 0, 0);
    }
    // c std math func의 반환이니까 rad로 나온다
    T theta = acos(z/r);

    T pi = 0;
    if (x != 0) {
      // x가 0이면 y/x를 계산할수 없다.
      pi = atan(y/x);
    } else {
      // x가 -이라는것은 pi의 각도는 90이거나 270이라는것이다.
      // 이것은 y의 부호에 따라 결정된다
      if (y > 0) {
        pi = Angle<T>::DegreeToRadian(static_cast<T>(90.0f));
      } else if (y == 0) {
        pi = Angle<T>::DegreeToRadian(static_cast<T>(0.0f));
      } else {
        pi = Angle<T>::DegreeToRadian(static_cast<T>(270.0f));
      }
    }
    return CreateRadianBase(r, theta, pi);
  }

  template<typename T>
  Rectangular3Point<T>::Rectangular3Point(T x, T y, T z)
    : x(x), y(y), z(z) {
  }
  template<typename T>
  Rectangular3Point<T> Rectangular3Point<T>::Create(
    const SphericalPoint<T> &o) {
    // TODO(if1live)
    // 구면극좌표를 변환하는 일은 아직 없다. 필요해지면 그떄 구현하자
    SR_ASSERT(!"TODO: impl");
  }
}
#endif  // BASE_MATSU_COORDINATE_IMPL_H_