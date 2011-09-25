#pragma once

#include "sora/macro.h"

namespace matsu
{
  template<typename T>
  Angle<T>::Angle(T degree)
  {
    //degree = -180~180사이의 범위가 되로록 유지하기
    if(degree >= -180 && degree <= 180) {
      degree_ = degree;
    } else if(degree > 180) {
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
  Angle<T>::~Angle()
  {
  }

  template<typename T>
  Angle<T> Angle<T>::Degree(T degree)
  {
    return Angle(degree);
  }
  template<typename T>
  Angle<T> Angle<T>::Radian(T radian)
  {
    return Angle(RadianToDegree(radian));
  }

  template<typename T>
  T Angle<T>::DegreeToRadian(T degree)
  {
    return degree * MT_PI / 180;
  }
  template<typename T>
  T Angle<T>::RadianToDegree(T radian)
  {
    return radian * 180 / MT_PI;
  }

  template<typename T>
  T Angle<T>::GetDegree()
  {
    return degree_;
  }
  template<typename T>
  T Angle<T>::GetRadian()
  {
    return DegreeToRadian(degree_);
  }

  template<typename T>
  bool Angle<T>::operator==(const Angle<T> &o)
  {
    if(o.degree_ == degree_) {
      return true;
    } else {
      return false;
    }
  }
  template<typename T>
  bool Angle<T>::operator!=(const Angle<T> &o)
  {
    return !(*this == o);
  }
}