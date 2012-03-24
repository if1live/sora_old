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
#include "sora_stdafx.h"
#include "sora/core/math_helper.h"

namespace sora {;
float DegToRad(float degree) {
  return SR_DEG_2_RAD(degree);
}
float RadToDeg(float radian) {
  return SR_RAD_2_DEG(radian);
}

bool EqualAbsError(float a, float b, float maxError) {
  if(a == b) {
    return true;
  }
  if(abs(a - b) <= maxError) {
    return true;
  }
  return false;
}

bool EqualRelError(float a, float b, float maxError) {
  if(fabs(a - b) < maxError) {
    return true;
  }
  float relativeError = 0;
  if(abs(b) > abs(a)) {
    relativeError = abs((a-b) / b);
  }	else {
    relativeError = abs((a-b) / a);
  }
  if(relativeError <= maxError) {
    return true;
  }
  return false;
}

bool EqualUlps(float a, float b, int maxUlps) {
  //http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
  SR_ASSERT(sizeof(float) == sizeof(int));
  if (a == b) {
    return true;
  }
  int intDiff = abs(*(int*)&a - *(int*)&b);
  if (intDiff <= maxUlps) {
    return true;
  }
  return false;
}
}
