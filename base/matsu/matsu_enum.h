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
#ifndef BASE_MATSU_MATSU_ENUM_H_
#define BASE_MATSU_MATSU_ENUM_H_

namespace matsu {
const float kPi = 3.141592f;
const float kPiOver2 = kPi / 2;
const float kPiOver4 = kPi / 4;

/// @brief 축과 관련된 내용은 왠지 언젠가 다시 쓸거같아서..
typedef enum {
  kAxisDirectionX,
  kAxisDirectionY,
  kAxisDirectionZ
} AxisDirection;

/// @brief 2차원에서 방향을 표현하는데 쓰이는것
typedef enum {
  kDirection2Left,
  kDirection2Right,
  kDirection2Up,
  kDirection2Down
} Direction2;

/// @brief 3차원에서 방향을 표현하는데 쓰이는것
typedef enum {
  kDirection3Left,
  kDirection3Right,
  kDirection3Top,
  kDirection3Bottom,
  kDirection3Front,
  kDirection3Back
} Direction3;

template<typename T, int D> class VectorTemplate;
typedef VectorTemplate<int, 2> ivec2;
typedef VectorTemplate<float, 2> vec2;
typedef VectorTemplate<int, 3> ivec3;
typedef VectorTemplate<float, 3> vec3;
typedef VectorTemplate<int, 4> ivec4;
typedef VectorTemplate<float, 4> vec4;


template<typename T, int row, int col>  class MatrixTemplate;
typedef MatrixTemplate<int, 2, 2> imat2;
typedef MatrixTemplate<float, 2, 2> mat2;
typedef MatrixTemplate<int, 3, 3> imat3;
typedef MatrixTemplate<float, 3, 3> mat3;
typedef MatrixTemplate<int, 4, 4> imat4;
typedef MatrixTemplate<float, 4, 4> mat4;

template<typename T> class Rectangle;
typedef Rectangle<int> Recti;
typedef Rectangle<float> Rectf;

template<typename T>  class Range;
typedef Range<int> Rangei;
typedef Range<float> Rangef;
}
#endif  // BASE_MATSU_MATSU_ENUM_H_
