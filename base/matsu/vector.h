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
#ifndef BASE_MATSU_VECTOR_H_
#define BASE_MATSU_VECTOR_H_


/// @file vector의 사용을 한방으로 하기 위해서+하위 호환성을 위해서 남겼다
#include <cmath>
#include "matsu/vector_template.h"

namespace matsu {;
typedef VectorTemplate<int, 2> ivec2;
typedef VectorTemplate<float, 2> vec2;

typedef VectorTemplate<int, 3> ivec3;
typedef VectorTemplate<float, 3> vec3;

typedef VectorTemplate<int, 4> ivec4;
typedef VectorTemplate<float, 4> vec4;
}
#endif  // BASE_MATSU_VECTOR_H_
