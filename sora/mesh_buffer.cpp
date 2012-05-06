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
#include "mesh_buffer.h"
#include "template_lib.h"

namespace sora {;
static int vertex_buffer_uid = 1;
VertexBufferInterface::VertexBufferInterface() : uid_(vertex_buffer_uid++) {
}

static int index_buffer_uid = 1;
IndexBufferInterface::IndexBufferInterface() : uid_(index_buffer_uid++) {
}

MeshBuffer::MeshBuffer() {}
MeshBuffer::~MeshBuffer() {
  sora::DestroyDict(&vertex_buffer_list_);
  sora::DestroyDict(&index_buffer_list_);
}

int MeshBuffer::Register(VertexBufferInterface *vb) {
  int uid = vb->uid();
  vertex_buffer_list_[uid] = vb;
  return uid;
}
int MeshBuffer::Register(IndexBufferInterface *ib) {
  int uid = ib->uid();
  index_buffer_list_[uid] = ib;
  return uid;
}
VertexBufferInterface *MeshBuffer::VertexBuffer(int uid) {
  auto found = vertex_buffer_list_.find(uid);
  if(found == vertex_buffer_list_.end()) {
    //null object
    static VertexArrayT<Vertex> empty;
    return &empty;
  } else {
    return found->second;
  }
}
IndexBufferInterface *MeshBuffer::IndexBuffer(int uid) {
  auto found = index_buffer_list_.find(uid);
  if(found == index_buffer_list_.end()) {
    //null object
    static IndexArray empty;
    return &empty;
  } else {
    return found->second;
  }
}
bool MeshBuffer::IsVertexBufferExist(int uid) const {
  auto found = vertex_buffer_list_.find(uid);
  return (found != vertex_buffer_list_.end());
}
bool MeshBuffer::IsIndexBufferExist(int uid) const {
  auto found = index_buffer_list_.find(uid);
  return (found != index_buffer_list_.end());
}

void MeshBuffer::AddDrawCmd(DrawType draw_mode, int vert, int index) {
  MeshDrawCommand cmd;
  cmd.draw_mode = draw_mode;
  cmd.vertex_buffer_handle = vert;
  cmd.index_buffer_handle = index;
  cmd_list_.push_back(cmd);
}

} // namespace sora