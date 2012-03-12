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
#ifndef SORA_MATH_HELPER_H_
#define SORA_MATH_HELPER_H_

#include "sora/arch.h"
#include <limits>
#include <cstdlib>
#include <cmath>

#define SR_PI (3.141592f)
#define SR_PI_OVER_2  (SR_PI/2.0f)
#define SR_PI_OVER_4  (SR_PI/4.0f)
#define SR_DEG_2_RAD(DEG) (DEG * SR_PI / 180.0f)
#define SR_RAD_2_DEG(RAD) (RAD * 180.0f / SR_PI)

namespace sora {;
const float kPi = SR_PI;
const float kPiOver2 = SR_PI_OVER_2;
const float kPiOver4 = SR_PI_OVER_4;

template<typename T>
bool IsNaN(T value) {
  if (value != value) {
    return true;
  } else {
    return false;
  }
}

template<typename T>
bool IsInf(T num) {
	if(std::numeric_limits<T>::has_infinity == false)	{
		return false;
	}
	if(num == std::numeric_limits<T>::infinity())	{
		return true;
	}
	return false;
}

SR_C_DLL float DegToRad(float degree);
SR_C_DLL float RadToDeg(float radian);

SR_C_DLL bool EqualAbsError(float a, float b, float maxError);	
SR_C_DLL bool EqualRelError(float a, float b, float maxError);	
SR_C_DLL bool EqualUlps(float a, float b, int maxUlps);

}

#endif  // SORA_MATH_HELPER_H_