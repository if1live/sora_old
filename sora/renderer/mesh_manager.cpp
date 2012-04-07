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
}

bool MeshManager::AddWire(const ISurface &surface, const char *name) {
  vector<Vertex> vert_list;
  vector<unsigned short> index_list;
  surface.GenerateVertices(vert_list);
  surface.GenerateLineIndices(index_list);

  DrawCommand<Vertex> draw_cmd;
  draw_cmd.draw_mode = GL_LINES;
  draw_cmd.index_count = index_list.size();
  draw_cmd.vert_count = vert_list.size();
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.vert_ptr = &vert_list[0];
  draw_cmd.index_ptr = &index_list[0];

  vector< DrawCommand<Vertex> > draw_cmd_list;
  draw_cmd_list.push_back(draw_cmd);
  return Add(draw_cmd_list, name);
}

MeshBufferObject *MeshManager::Get(const std::string &name) {
  auto found = mesh_dict_.find(name);
  if(found == mesh_dict_.end()) {
    return NULL;
  } else {
    return &found->second;
  }
}
bool MeshManager::IsExist(const char *name) const {
  std::string name_str(name);
  auto found = mesh_dict_.find(name_str);
  if(found == mesh_dict_.end()) {
    return false;
  } else {
    return true;
  }
}
bool MeshManager::Add(const MeshBufferObject &obj, const char *name) {
  if(IsExist(name) == true) {
    return false;
  }
  mesh_dict_[string(name)] = obj;
  return true;
}
}
