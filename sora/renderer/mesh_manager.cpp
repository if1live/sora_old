﻿/*  Copyright (C) 2011-2012 by if1live */
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

#include "parametric_equations.h"
#include "parametric_surface.h"

using namespace std;

namespace sora {;
MeshManager::MeshManager() {
}
MeshManager::~MeshManager() {
}

bool MeshManager::AddSurface_line(const ISurface &surface, const char *name) {
  vector<Vertex> vert_list;
  vector<unsigned short> index_list;
  surface.GenerateVertices(vert_list);
  surface.GenerateTriangleIndices(index_list);

  DrawCommand draw_cmd;
  draw_cmd.draw_mode = GL_TRIANGLES;
  draw_cmd.index_count = index_list.size();
  draw_cmd.vert_count = vert_list.size();
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.vert_ptr = &vert_list[0];
  draw_cmd.index_ptr = &index_list[0];

  vector<DrawCommand> draw_cmd_list;
  draw_cmd_list.push_back(draw_cmd);
  return Add(draw_cmd_list, name);
}

bool MeshManager::AddSurface_triangle(const ISurface &surface, const char *name) {
  vector<Vertex> vert_list;
  vector<unsigned short> index_list;
  surface.GenerateVertices(vert_list);
  surface.GenerateLineIndices(index_list);

  DrawCommand draw_cmd;
  draw_cmd.draw_mode = GL_LINES;
  draw_cmd.index_count = index_list.size();
  draw_cmd.vert_count = vert_list.size();
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.vert_ptr = &vert_list[0];
  draw_cmd.index_ptr = &index_list[0];

  vector<DrawCommand> draw_cmd_list;
  draw_cmd_list.push_back(draw_cmd);
  return Add(draw_cmd_list, name);
}

bool MeshManager::Add(const std::vector<DrawCommand> &cmd_list, const char *name) {
  if(IsExist(name) == true) {
    return false;
  }

  MeshBufferObject buffer_obj;
  auto it = cmd_list.begin();
  auto endit = cmd_list.end();
  for( ; it != endit ; ++it) {
    buffer_obj.Add(*it);
  }
  mesh_dict_[string(name)] = buffer_obj;
  return true;
}

MeshBufferObject *MeshManager::Get(const char *name) {
  std::string name_str(name);
  auto found = mesh_dict_.find(name_str);
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
}
