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
Material::Material() 
  : shininess(10.0f), props(0) {
    //mtl format default value
    for(int i = 0 ; i < 4 ; i++) {
      //ambient[i] = 0.2f;
      ambient[i] = 1.0f;
      diffuse[i] = 0.8f;
      specular[i] = 1.0f;
    }
};

const Material &Material::NullMaterial() {
  static Material mtl;
  mtl.props = 0;  //0이면 플래그 검사 그냥 통과하니까 전부 disable의미
  return mtl;
}

bool Material::operator==(const Material &o) const {
  if(name != o.name) { return false; }
  if(props != o.props) { return false; }
  if(ambient != o.ambient) { return false; }
  if(diffuse != o.diffuse) { return false; }
  if(specular != o.specular) { return false; }
  if(shininess != o.shininess) { return false; }
  if(diffuse_map != o.diffuse_map) { return false; }
  if(specular_map != o.specular_map) { return false; }
  if(normal_map != o.normal_map) { return false; }
  return true;
}

/////////////////////////////////////
MaterialManager::MaterialManager() {}
MaterialManager::~MaterialManager() {}

bool MaterialManager::IsExist(const std::string &name) const {
  auto found = material_list_.find(name);
  if(found == material_list_.end()) {
    return false;
  } else {
    return true;
  }
}

bool MaterialManager::Add(const std::vector<Material> &mtl_list) {
  auto it = mtl_list.begin();
  auto endit = mtl_list.end();
  bool collision_occur = false;
  for( ; it != endit ; ++it) {
    bool add_result = Add(*it);
    if(add_result == false) {
      collision_occur = true;
    }
  }
  return collision_occur;
}
bool MaterialManager::Add(const Material &mtl) {
  if(IsExist(mtl.name) == true) {
    //name collision
    return false;
  }
  material_list_[mtl.name] = mtl;
  return true;
}
const Material &MaterialManager::Get(const std::string &name) const {
  auto found = material_list_.find(name);
  if(found == material_list_.end()) {
    //else..not found, use basic material
    static Material basic_material;
    return basic_material;
  }
  return found->second;
}
void MaterialManager::Clear() {
  material_list_.clear();
}

} //namespace sora

