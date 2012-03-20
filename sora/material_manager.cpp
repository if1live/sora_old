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
#include "material_manager.h"
#include "template_lib.h"

#include "filesystem.h"
#include "memory_file.h"
#include "obj_loader.h"

using namespace std;

namespace sora {;
struct MaterialManagerImpl {
  //일단은 몇개 안될테니까 간단하게 구현
  //재질정보는 그렇게 크지도 않고 많지도 않을테니까 전부 떄려박아도 심각한 문제가
  //발생하지는 않을것이다.
  std::vector<Material> material_list;
};

MaterialManager::MaterialManager() : impl(new MaterialManagerImpl()) {
}
MaterialManager::~MaterialManager() {
  SafeDelete(impl);
}

bool MaterialManager::IsExist(const std::string &name) const {
  auto it = impl->material_list.begin();
  auto endit = impl->material_list.end();
  for( ; it != endit ; ++it) {
    const Material &mtl = *it;
    if(name == mtl.name) {
      return true;
    }
  }
  return false;
}
bool MaterialManager::IsExist(const char *name) const {
  return IsExist(string(name));
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
  impl->material_list.push_back(mtl);
}
const Material &MaterialManager::Get(const std::string &name) {
  auto it = impl->material_list.begin();
  auto endit = impl->material_list.end();
  for( ; it != endit ; ++it) {
    if(name == it->name) {
      return *it;
    }
  }
  //else..not found, use basic material
  static Material basic_material;
  return basic_material;
}
const Material &MaterialManager::Get(const char *name) {
  return Get(string(name));
}

bool MaterialManager::LoadFromFile() {
  impl->material_list.clear();

  std::string mtl_path = Filesystem::GetAppPath("material/example.mtl");
  MemoryFile mtl_file(mtl_path);
  mtl_file.Open();
  vector<sora::Material> material_list;
  ObjLoader loader;
  loader.LoadMtl(mtl_file.start, mtl_file.end, &material_list);
  Add(material_list);
  return true;
}
}

SR_C_DLL sora::MaterialManager &MaterialManger_get_instance() {
  return sora::MaterialManager::GetInstance();
}