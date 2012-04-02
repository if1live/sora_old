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
#include "material.h"

namespace sora {;
Material::Material() {
  //mtl format default value
  for(int i = 0 ; i < 3 ; i++) {
    ambient[i] = 0.2f;
    diffuse[i] = 0.8f;
    specular[i] = 1.0f;
  }
  alpha = 1.0f;  //not transparent
  shininess = 10.0f;

  uber_flag = 0;
};

const Material &Material::GetInvalidMtl() {
  static Material mtl;
  mtl.uber_flag = kInvalidShaderFlag;
  return mtl;
}

bool Material::operator==(const Material &o) const {
  if(name != o.name) { return false; }
  if(uber_flag != o.uber_flag) { return false; }
  if(ambient != o.ambient) { return false; }
  if(diffuse != o.diffuse) { return false; }
  if(specular != o.specular) { return false; }
  if(shininess != o.shininess) { return false; }
  if(ambient_map != o.ambient_map) { return false; }
  if(diffuse_map != o.diffuse_map) { return false; }
  if(specular_map != o.specular_map) { return false; }
  if(normal_map != o.normal_map) { return false; }
  return true;
}
}