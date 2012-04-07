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
#ifndef SORA_MESH_MANAGER_H_
#define SORA_MESH_MANAGER_H_

#include "mesh.h"

#if SR_USE_PCH == 0
#include "core/unordered_map_inc.h"
#endif

namespace sora {;

struct ISurface;  //for parametric surface

class MeshManager {
public:
  MeshManager();
  ~MeshManager();

  //단순선은 신경 안쓰고 그냥 vertex로 생성하면 된다
  bool AddWire(const ISurface &surface, const char *name);

  template<typename VertexType>
  bool AddSolid(const ISurface &surface, const char *name);

  template<typename VertexType>
  bool Add(const std::vector< DrawCommand<VertexType> > &cmd_list, const char *name);
  bool Add(const MeshBufferObject &obj, const char *name);

  MeshBufferObject *Get(const char *name) { return Get(std::string(name)); }
  MeshBufferObject *Get(const std::string &name);
  bool IsExist(const char *name) const;

private:
  typedef std::tr1::unordered_map<std::string, MeshBufferObject> MeshDictType;
  MeshDictType mesh_dict_;
};

template<typename VertexType>
bool MeshManager::AddSolid(const ISurface &surface, const char *name) {
  std::vector<VertexType> vert_list;
  surface.GenerateVertices(vert_list);

  std::vector<unsigned short> index_list;
  surface.GenerateTriangleIndices(index_list);

  DrawCommand<VertexType> draw_cmd;
  draw_cmd.draw_mode = GL_TRIANGLES;
  draw_cmd.index_count = index_list.size();
  draw_cmd.vert_count = vert_list.size();
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.vert_ptr = &vert_list[0];
  draw_cmd.index_ptr = &index_list[0];

  std::vector< DrawCommand<VertexType> > draw_cmd_list;
  draw_cmd_list.push_back(draw_cmd);
  return Add(draw_cmd_list, name);
}

template<typename VertexType>
bool MeshManager::Add(const std::vector< DrawCommand<VertexType> > &cmd_list, const char *name) {
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

}

#endif  // SORA_MESH_MANAGER_H_