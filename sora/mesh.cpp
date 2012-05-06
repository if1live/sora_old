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
#include "mesh.h"
#include "mesh_buffer.h"
#include "template_lib.h"
#include "buffer_object.h"

namespace sora {;

MeshElem::MeshElem(MeshBuffer *buffer, const Material &mtl)
  : mesh_buffer(buffer), mtl(mtl) {

}
MeshElem::~MeshElem() {
  if(mesh_buffer != NULL) {
    delete(mesh_buffer); 
    mesh_buffer = NULL;
  }
}
Mesh::Mesh() {

}
Mesh::~Mesh() {

}

void Mesh::Register(MeshBuffer *mesh_buffer, const Material &mtl) {
  MeshElemPtr elem_ptr(new MeshElem(mesh_buffer, mtl));
  elem_list_.push_back(elem_ptr);
}

void Mesh::Register(MeshBuffer *mesh_buffer) {
  Material null_mtl = Material::NullMaterial();
  Register(mesh_buffer, null_mtl);
}
void Mesh::Register(const std::vector<DrawCmdData<Vertex>> &cmd_list, const Material &mtl) {
  auto it = cmd_list.begin();
  auto endit = cmd_list.end();
  for( ; it != endit ; ++it) {
    const DrawCmdData<Vertex> &cmd = *it;

    VertexBufferObject *vbo = new VertexBufferObject();
    vbo->Init(cmd.vertex_list);

    IndexBufferObject *ibo = new IndexBufferObject();
    ibo->Init(cmd.index_list);

    MeshBuffer *mesh_buffer = new MeshBuffer();
    int vbo_id = mesh_buffer->Register(vbo);
    int ibo_id = mesh_buffer->Register(ibo);
    mesh_buffer->AddDrawCmd(cmd.draw_mode, cmd.disable_cull_face, vbo_id, ibo_id);

    Register(mesh_buffer, mtl);
  }
}
void Mesh::Register(const std::vector<DrawCmdData<Vertex>> &cmd_list) {
  Material null_mtl = Material::NullMaterial();
  Register(cmd_list, null_mtl);
}
////////////////////////////////////////

MeshManager::MeshManager() {}
MeshManager::~MeshManager() {
  DestroyDict(&mesh_dict_);
}

bool MeshManager::Add(const std::string &name, Mesh *mesh) {
  if(IsExist(name)) {
    return false;
  }
  mesh_dict_[name] = mesh;
  return true;
}

Mesh *MeshManager::Get(const std::string &name) {
  auto found = mesh_dict_.find(name);
  if(found == mesh_dict_.end()) {
    static Mesh empty;
    return &empty;
  } else {
    return found->second;
  }
}
bool MeshManager::IsExist(const std::string &name) const {
  auto found = mesh_dict_.find(name);
  return (found != mesh_dict_.end()); 
}

} //namespace sora