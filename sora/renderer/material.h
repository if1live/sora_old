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
#pragma once

#include "globals.h"

namespace sora {;
struct Material {
  Material();
  const Material &NullMaterial();

  std::string name;
  uint props;   //flag저장 용도

  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  float shininess;  //specular

  std::string ambient_map;
  std::string diffuse_map;
  std::string specular_map;
  std::string normal_map;

  bool operator==(const Material &o) const;
  bool operator!=(const Material &o) const {
    return !(*this == o);
  }
};

class MaterialManager {
public:
  MaterialManager();
  ~MaterialManager();

public:
  //이름중복이 발생한 경우 false
  bool Add(const std::vector<Material> &mtl_list);
  bool Add(const Material &mtl);
  bool IsExist(const std::string &name) const;
  const Material &Get(const std::string &name) const;
  void Clear();

private:
  //일단은 몇개 안될테니까 간단하게 구현
  //재질정보는 그렇게 크지도 않고 많지도 않을테니까 전부 떄려박아도 심각한 문제가
  //발생하지는 않을것이다.
  typedef std::tr1::unordered_map<std::string, Material> MaterialDict;
  MaterialDict material_list_;
};
} //namespace sora
