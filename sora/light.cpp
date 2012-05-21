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
#include "light.h"

#include "memory_file.h"
#include "filesystem.h"

using namespace std;

namespace sora {;
Light::Light()
  : ambient(1.0f, 1.0f, 1.0f, 1.0f),
  diffuse(1.0f, 1.0f, 1.0f, 1.0f),
  specular(1.0f, 1.0f, 1.0f, 1.0f),
  type(kLightPoint),
  radius(0),
  inner_cone(20),
  outer_cone(30) {
}
void Light::SetPoint(const glm::vec3 &pos, float radius) {
  type = kLightPoint;
  this->pos = pos;
  this->radius = radius;
}
void Light::SetDirection(const glm::vec3 &dir) {
  type = kLightDirection;
  this->dir = dir;
}
void Light::SetSpotLight(const glm::vec3 &pos, const glm::vec3 &dir, float inner, float outer) {
  type = kLightSpotLight;
  this->pos = pos;
  this->dir = dir;
  this->inner_cone = inner;
  this->outer_cone = outer;
}

const std::string &Light::GetLightModelFragSrc() {
  static string light_src;
  if(light_src.empty()) {
    const char *light_model_frag_file = "shader/light_model.fs";
    string file_path = Filesystem::GetAppPath(light_model_frag_file);
    MemoryFile mem_file(file_path);
    bool opened = mem_file.Open();
    SR_ASSERT(opened == true);
    light_src = (const char*)mem_file.start;
    mem_file.Close();
  }
  return light_src;
}
}

