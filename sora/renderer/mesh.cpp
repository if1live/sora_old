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
#include "gl_helper.h"

using namespace std;

namespace sora {;
MeshBufferObject::MeshBufferObject() : vertex_type_(kNoVertex) {
}
MeshBufferObject::~MeshBufferObject() {
}

void MeshBufferObject::Deinit() {
  for(size_t i = 0 ; i < vbo_list_.size() ; ++i) {
    VertexBufferObject &vbo = vbo_list_[i];
    IndexBufferObject &ibo = ibo_list_[i];
    vbo.Deinit();
    ibo.Deinit();
  }
  vbo_list_.clear();
  ibo_list_.clear();
  index_count_list_.clear();
  draw_mode_list_.clear();
}

int MeshBufferObject::BufferCount() const {
  SR_ASSERT(vbo_list_.size() == ibo_list_.size());
  SR_ASSERT(ibo_list_.size() == index_count_list_.size());
  SR_ASSERT(index_count_list_.size() == draw_mode_list_.size());
  return vbo_list_.size();
}

VertexBufferObject &MeshBufferObject::vbo(int idx) {
  SR_ASSERT(idx >= 0);
  SR_ASSERT(idx < BufferCount());
  return vbo_list_[idx];
}

IndexBufferObject &MeshBufferObject::ibo(int idx) {
  SR_ASSERT(idx >= 0);
  SR_ASSERT(idx < BufferCount());
  return ibo_list_[idx];
}

const VertexBufferObject &MeshBufferObject::vbo(int idx) const {
  SR_ASSERT(idx >= 0);
  SR_ASSERT(idx < BufferCount());
  return vbo_list_[idx];
}

const IndexBufferObject &MeshBufferObject::ibo(int idx) const {
  SR_ASSERT(idx >= 0);
  SR_ASSERT(idx < BufferCount());
  return ibo_list_[idx];
}

int MeshBufferObject::index_count(int idx) const {
  SR_ASSERT(idx >= 0);
  SR_ASSERT(idx < BufferCount());
  return index_count_list_[idx];
}

GLenum MeshBufferObject::draw_mode(int idx) const {
  SR_ASSERT(idx >= 0);
  SR_ASSERT(idx < BufferCount());
  return draw_mode_list_[idx];
}

}
