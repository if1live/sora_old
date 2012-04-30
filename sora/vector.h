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
#ifndef SORA_VECTOR_H_
#define SORA_VECTOR_H_

#include <glm/glm.hpp>

//glm의 vector에는 unsigned char에 대응되는 것이 없다
//왜나하면 저건 gl스펙에 맞춘거니까. 하지만 데이터를 최적화 하기 위해서는
//자주 쓸거같아서 glm비슷하게해서 없는거 몇개를 뚫엇다
namespace sora {;
typedef glm::detail::tvec2<unsigned char> vec2ub;
typedef glm::detail::tvec3<unsigned char> vec3ub;
typedef glm::detail::tvec4<unsigned char> vec4ub;
}

#endif  // SORA_VECTOR_H_