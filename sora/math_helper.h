// Copyright (C) 2011 by if1live
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

#include "vector.h"
#include "matrix.h"
#include "arch.h"

#if SR_USE_PCH == 0
#include <cmath>
#include <limits>
#endif

namespace sora {;
const f32 kPi = 3.141592f;
const f32 kPiOver2 = kPi / 2;
const f32 kPiOver4 = kPi / 4;

inline f32 DegreeToRadian(f32 degree) {
  return degree * kPi / 180.0f;
}
inline f32 RadianToDegree(f32 radian) {
  return radian * 180.0f / kPi;
}

template<typename T, i32 D>
void SetIdentity(Matrix<T, D, D> *m) {
  memset(m->value, 0, sizeof(T) * D * D);
  for (i32 i = 0 ; i < D ; i++) {
    m->Set(i, i, 1);
  }
}


bool EqualAbsError(float a, float b, float maxError);	
bool EqualRelError(float a, float b, float maxError);	
bool EqualUlps(float a, float b, int maxUlps);

template<typename T>
boolean IsNaN(T value) {
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

template<typename T>
bool IsNormalNumber(T num) {
	if(IsNaN(num) || IsInf(num)) 	{
		return false;
	}
	return true;
}

template<typename T>
bool IsPower(T base, T target) {
	if(target < 1) {
		//2*-1=0.5인데 이런 경우는 쓰지 않을듯해서 신경쓰지 않는다
		//어차피 위와같이 분수로 내려가는 경우, 정밀도 문제로
		//쓰지도 못할거같다
		return false;
	}
	T curr = base;
	while(curr <= target) {
		if(curr == target) {
			return true;
		}
		curr *= base;
	}
	return false;
}

template<typename T>
T CeilPower(T base, T target) {
	if(target < 1) {
		//소수점이하는 계산무시
		return base;
	}
	T curr = base;
	while(curr < target) {
		curr *= base;
	}
	return curr;
}

template<typename T>
T CalcIncludeAngleDegree(T src, T dst) {
	T radResult = CalcIncludeAngleRadian(DegreeToRadian(src), DegreeToRadian(dst));
	return RadianToDegree(radResult);
}
	
template<typename T>
T CalcIncludeAngleRadian(T src, T dst) {
	vec2 srcVec2(cos(src), sin(src));
	vec2 dstVec2(cos(dst), sin(dst));
	float vecDot = VectorDot(srcVec2, dstVec2);
	float srcSize = VectorLength(srcVec2);
	float dstSize = VectorLength(dstVec2);
	float value = vecDot / srcSize / dstSize;
	if(value >= 1.0f) {
		value = 1.0f;
	} else if(value <= -1.0f) {
		value = -1.0f;
	}
	float rawAbsRadian = acos(value);
	SR_ASSERT(IsNaN(rawAbsRadian) == false);
		
	//벡터의 외적을 이용해서 zero-curr이 양수방향인지 음수방향인지를 결정한다
	vec3 srcVec3(srcVec2);
	vec3 dstVec3(dstVec2);
	vec3 crossPan = VectorCross(srcVec3, dstVec3);
	if(crossPan.z > 0) {
		return rawAbsRadian;
	} else {
		return -rawAbsRadian;
	}
}

template<typename T>
T Lerp(T from, T to, float t) {
	return from * (1-t) + to * t;
}

template<typename T, i32 D>
boolean IsIdentity(const Matrix<T, D, D> &m) {
  for (i32 x = 0 ; x < D ; x++) {
    for (i32 y = 0 ; y < D ; y++) {
      T elem = m.Get(x, y);
      if (x == y) {
        if (elem != 1) {
          return false;
        }
      } else {
        if (elem != 0) {
          return false;
        }
      }
    }
  }
  return true;
}

template<typename T>
void SetTranslate(T x, T y, T z, Matrix<T, 4, 4> *m) {
 T data[] = {
   1, 0, 0, 0,
   0, 1, 0, 0,
   0, 0, 1, 0,
   x, y, z, 1
  };
 m->Set(data);
}

template<typename T>
void SetScale(T x, T y, T z, Matrix<T, 4, 4> *m) {
  T data[] = {
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1
  };
  m->Set(data);
}
template<typename T>
void SetScale(T s, Matrix<T, 4, 4> *m) {
  SetScale(s, s, s, m);
}

template <typename T>
void SetRotateX(T degrees, Matrix<T, 4, 4> *m) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
      1, 0, 0, 0,
      0, c, s, 0,
      0, -s, c, 0,
      0, 0, 0, 1
  };
  m->Set(data);
}
template <typename T>
void SetRotateY(T degrees, Matrix<T, 4, 4> *m) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
      c, 0, -s, 0,
      0, 1, 0, 0,
      s, 0, c, 0,
      0, 0, 0, 1,
  };
  m->Set(data);
}
template <typename T>
void SetRotateZ(T degrees, Matrix<T, 4, 4> *m) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
      c, s, 0, 0,
      -s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  };
  m->Set(data);
}

mat4 &SetOrtho(float left, float right, float bottom, float top, float near_val, float far_val, mat4 *m);
mat4 &SetFrustum(float left, float right, float bottom, float top, float near_val, float far_val, mat4 *m);
mat4 &SetLookAt(float eye_x, float eye_y, float eye_z,
  float target_x, float target_y, float target_z,
  float up_x, float up_y, float up_z, mat4 *m);
mat4 &SetPerspective(float fovy, float aspect, float zNear, float zFar, mat4 *m);
}

#endif  // SORA_MATH_HELPER_H_
