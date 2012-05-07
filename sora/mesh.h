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
#pragma once

#include "material.h"
#include "vertex.h"

namespace sora {;

class MeshBuffer;
class Material;

struct MeshElem;
typedef std::tr1::shared_ptr<MeshElem> MeshElemPtr;
struct MeshElem {
  MeshElem(MeshBuffer *buffer, const Material &mtl);
  ~MeshElem();
  MeshBuffer *mesh_buffer;
  Material mtl;
};

class Mesh {
public:
  Mesh();
  ~Mesh();

  void Register(MeshBuffer *mesh_buffer, const Material &mtl);
  void Register(MeshBuffer *mesh_buffer);

  template<typename T>
  void Register(const std::vector<DrawCmdData<T>> &cmd_list, const Material &mtl);
  template<typename T>
  void Register(const std::vector<DrawCmdData<T>> &cmd_list);

  std::vector<MeshElemPtr>::iterator Begin() { return elem_list_.begin(); }
  std::vector<MeshElemPtr>::iterator End() { return elem_list_.end(); }
private:
  std::vector<MeshElemPtr> elem_list_;
};


class MeshManager {
public:
  MeshManager();
  ~MeshManager();
  bool Add(const std::string &name, Mesh *mesh);
  //같은 이름이 없으면 빈 메시를 반환
  Mesh *Get(const std::string &name);
  bool IsExist(const std::string &name) const;

private:
  typedef std::tr1::unordered_map<std::string, Mesh*> MeshDict;
  MeshDict mesh_dict_;
};


template<typename T>
void Mesh::Register(const std::vector<DrawCmdData<T>> &cmd_list, const Material &mtl) {
  auto it = cmd_list.begin();
  auto endit = cmd_list.end();
  for( ; it != endit ; ++it) {
    const DrawCmdData<T> &cmd = *it;

    VertexBufferInterface *vbo = new VertexBufferObjectT<T>();
    bool vbo_init_result = vbo->Init(cmd.vertex_list);
    SR_ASSERT(vbo_init_result == true);

    IndexBufferInterface *ibo = NULL;
    if(cmd.index_list.empty() == false) {
      ibo = new IndexBufferObject();
      bool ibo_init_result = ibo->Init(cmd.index_list);
      SR_ASSERT(ibo_init_result == true);
    }

    MeshBuffer *mesh_buffer = new MeshBuffer();
    int vbo_id = mesh_buffer->Register(vbo);
    if(ibo != NULL) {
      int ibo_id = mesh_buffer->Register(ibo);
      mesh_buffer->AddDrawCmd(cmd.draw_mode, cmd.disable_cull_face, vbo_id, ibo_id);
    } else {
      mesh_buffer->AddDrawCmd(cmd.draw_mode, cmd.disable_cull_face, vbo_id);
    }

    Register(mesh_buffer, mtl);
  }
}
template<typename T>
void Mesh::Register(const std::vector<DrawCmdData<T>> &cmd_list) {
  Material null_mtl = Material::NullMaterial();
  Register(cmd_list, null_mtl);
}
} //namespace sora