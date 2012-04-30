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
#ifndef SORA_LIGHT_H_
#define SORA_LIGHT_H_

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#endif

namespace sora {;

struct Light {
  Light()
    : ambient(1.0f, 1.0f, 1.0f, 1.0f),
    diffuse(1.0f, 1.0f, 1.0f, 1.0f),
    specular(1.0f, 1.0f, 1.0f, 1.0f) {
  }

  //shadow map를 구성하기 위해서는 사실상 카메라와 동일한 속성이 필요하다
  glm::vec3 pos;  //TODO pos의 4번쨰 값으로 directional인지 point인지를 구분하도록 고지치
  glm::vec3 center;
  glm::vec3 up;

  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  //float shininess;
};

}

#endif  // SORA_LIGHT_H_