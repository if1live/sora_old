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
#include "mesh_manager.h"

#include "mesh/parametric_equations.h"
#include "mesh/parametric_surface.h"

using namespace std;

namespace sora {;
MeshManager::MeshManager() {
}
MeshManager::~MeshManager() {
  auto it = mesh_dict_.begin();
  auto endit = mesh_dict_.end();
  for( ; it != endit ; ++it) {
    MeshBufferObject &mesh_obj = it->second;
    mesh_obj.Deinit();
  }
  mesh_dict_.clear();
}

MeshBufferObject *MeshManager::Get(const std::string &name) {
  auto found = mesh_dict_.find(name);
  if(found == mesh_dict_.end()) {
    return NULL;
  } else {
    return &found->second;
  }
}
bool MeshManager::IsExist(const std::string &name) const {
  auto found = mesh_dict_.find(name);
  if(found == mesh_dict_.end()) {
    return false;
  } else {
    return true;
  }
}
bool MeshManager::Add(const MeshBufferObject &obj, const std::string &name) {
  if(IsExist(name) == true) {
    return false;
  }
  mesh_dict_[string(name)] = obj;
  return true;
}
}
