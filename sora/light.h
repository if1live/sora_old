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

#include "globals.h"

namespace sora {;

struct Light {
  Light()
  : ambient(1.0f, 1.0f, 1.0f, 1.0f),
  diffuse(1.0f, 1.0f, 1.0f, 1.0f),
  specular(1.0f, 1.0f, 1.0f, 1.0f),
  type(kLightPoint),
  radius(0),
  inner_cone(20),
  outer_cone(30),
  model_mat(1.0f) {
  }
  void SetPoint(const glm::vec3 &pos, float radius) {
    type = kLightPoint;
    this->pos = pos;
    this->radius = radius;
  }
  void SetDirection(const glm::vec3 &dir) {
    type = kLightDirection;
    this->dir = dir;
  }
  void SetSpotLight(const glm::vec3 &pos, const glm::vec3 &dir, float inner, float outer) {
    type = kLightSpotLight;
    this->pos = pos;
    this->dir = dir;
    this->inner_cone = inner;
    this->outer_cone = outer;
  }

  glm::vec3 pos;
  glm::vec3 dir;
  float radius;
  float inner_cone;
  float outer_cone;
  

  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;

  LightType type;

  glm::mat4 model_mat;
};

}

#endif  // SORA_LIGHT_H_