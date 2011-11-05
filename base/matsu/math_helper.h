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
#ifndef BASE_MATSU_MATH_HELPER_H_
#define BASE_MATSU_MATH_HELPER_H_

#include <cmath>
#include <string>
#include "sora/macro.h"
#include "matsu/matsu_enum.h"

namespace matsu {;

template<typename T>  class MathHelperTemplate;
typedef MathHelperTemplate<float> MathHelper;

template<typename T>
class MathHelperTemplate {
 public:
  static bool EqualAbsoluteError(T a, T b, T maxError);
  static bool EqualRelativeError(T a, T b, T maxError);
  static bool EqualUlps(T a, T b, int maxUlps);

  /// @brief target이 base의 지수승으로 표현가능한 숫자인가
  static bool IsPower(T base, T target);

  /// @brief target보다 크거가 같은 base의 지수승으로 표현 가능한 숫자
  static T CeilPower(T base, T target);
  static bool IsNaN(T num);
  static bool IsINF(T num);

  /// @brief nan이나 무한대가 아닌수이면 true
  static bool IsNormalNumber(T num);

  /// @brief atoi, atof를 할때 스트링클래스를 직접 못넣고 c_str을 거쳐가는것이
  /// 매우 귀찮아서 만든 함수. 템플릿으로 만들어서 캐스팅 한번더 할필요 없이
  /// 원하는 타입을 얻을수 있도록 했다
  static int StringToInt(const std::string &str);
  static float StringToFloat(const std::string &str);
  static T CalculateIncludeAngleDegree(T src, T dst);
  static T CalculateIncludeAngleRadian(T src, T dst);

  template<typename T2>
  static T2 Lerp(T2 from, T2 to, float t);
};
/*
///@brief asin, acos의 경우는 입력이 -1~1이어야 정상적으로 작동한다
///정상범위인지 확인하기 위해서 오차를 받아서 대강 그 근처면 보정하고
///그것이 아니면 assert로 죽자
template<typename T>
T asinRad(T value, T maxError = 0.00001);
template<typename T>
T asinDeg(T value, T maxError = 0.00001);

template<typename T>
T acosRad(T value, T maxError = 0.00001);
template<typename T>
T acosDeg(T value, T maxError = 0.00001);

template<typename T>
T atanRad(T value);
template<typename T>
T atanDeg(T value);
 */
}

#include "matsu/math_helper_impl.h"

#endif  // BASE_MATSU_MATH_HELPER_H_
