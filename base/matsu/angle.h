#pragma once

#include "matsu/matsu_enum.h"

namespace matsu
{
  template<typename T>  class Angle;
  typedef Angle<float> Anglef;
  typedef Angle<int>  Anglei;

  template<typename T>
  class Angle {
  public:
    static T DegreeToRadian(T degree);
    static T RadianToDegree(T radian);

    static Angle Degree(T degree);
    static Angle Radian(T radian);
  public:

    Angle(T degree);
    ~Angle();

    bool operator==(const Angle<T> &o);
    bool operator!=(const Angle<T> &o);

    T GetDegree();
    T GetRadian();
  private:
    T degree_;
  };
}

#include "matsu/angle.hpp"