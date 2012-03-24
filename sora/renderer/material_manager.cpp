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
#include "core/template_lib.h"

#include "sys/filesystem.h"
#include "sys/memory_file.h"
#include "renderer/obj_loader.h"

using namespace std;

namespace sora {;
MaterialManager::MaterialManager() {
}
MaterialManager::~MaterialManager() {
}

bool MaterialManager::IsExist(const std::string &name) const {
  auto it = material_list_.begin();
  auto endit = material_list_.end();
  for( ; it != endit ; ++it) {
    const Material &mtl = *it;
    if(name == mtl.name) {
      return true;
    }
  }
  return false;
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
  material_list_.push_back(mtl);
}
const Material &MaterialManager::Get(const std::string &name) {
  auto it = material_list_.begin();
  auto endit = material_list_.end();
  for( ; it != endit ; ++it) {
    if(name == it->name) {
      return *it;
    }
  }
  //else..not found, use basic material
  static Material basic_material;
  return basic_material;
}
void MaterialManager::Clear() {
  material_list_.clear();
}

SR_C_DLL const Material &MaterialMgr_get(const std::string &name) {
  MaterialManager &mgr = MaterialManager::GetInstance();
  return mgr.Get(name);
}
SR_C_DLL bool MaterialMgr_is_exist(const std::string &name) {
  MaterialManager &mgr = MaterialManager::GetInstance();
  bool retval = mgr.IsExist(name);
  return retval;
}
SR_C_DLL bool MaterialMgr_initialize_from_file() {
  MaterialManager &mgr = MaterialManager::GetInstance();
  mgr.Clear();

  std::string mtl_path = Filesystem::GetAppPath("material/example.mtl");
  MemoryFile mtl_file(mtl_path);
  mtl_file.Open();
  vector<sora::Material> material_list;
  ObjLoader loader;
  loader.LoadMtl(mtl_file.start, mtl_file.end, &material_list);
  mgr.Add(material_list);
  return true;
}
}
