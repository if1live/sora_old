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
#ifndef SORA_MESH_H_
#define SORA_MESH_H_

#include "renderer/vertex.h"
#include "renderer/gl/gl_buffer_object.h"

#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

namespace sora {;

template<typename VertexType> struct DrawCommand;

//메시를 그리는데 필요한 정보. 렌더러는 이 정보를 얻어서 돌아가도록했다
template<typename VertexType>
struct DrawCommand {
  typedef VertexType VertType;
  DrawCommand() 
    : draw_mode(GL_TRIANGLES), 
    vert_ptr(NULL),
    index_ptr(NULL), 
    index_count(0),
    index_type(GL_UNSIGNED_SHORT) { }

  GLenum draw_mode;
  const VertexType *vert_ptr;
  const void *index_ptr;
  int index_count;
  int vert_count;
  GLenum index_type;
};

class MeshBufferObject {
public:
  MeshBufferObject();
  ~MeshBufferObject();

  template<typename T>
  void Add(const DrawCommand<T> &cmd);
  void Deinit();

  int BufferCount() const;
  VertexBufferObject &vbo(int idx);
  IndexBufferObject &ibo(int idx);
  const VertexBufferObject &vbo(int idx) const;
  const IndexBufferObject &ibo(int idx) const;
  int index_count(int idx) const;
  GLenum draw_mode(int idx) const;
  int vertex_type() const { return vertex_type_; }

private:
  std::vector<VertexBufferObject> vbo_list_;
  std::vector<IndexBufferObject> ibo_list_;
  std::vector<int> index_count_list_;
  std::vector<GLenum> draw_mode_list_;
  int vertex_type_;
};

template<typename T>
void MeshBufferObject::Add(const DrawCommand<T> &cmd) {
  VertexBufferObject vbo;
  vbo.Init(cmd.vert_count * sizeof(T), (void*)cmd.vert_ptr, GL_STATIC_DRAW);

  IndexBufferObject ibo;
  ibo.Init(cmd.index_count * sizeof(unsigned short), (void*)cmd.index_ptr, GL_STATIC_DRAW);

  vbo_list_.push_back(vbo);
  ibo_list_.push_back(ibo);
  index_count_list_.push_back(cmd.index_count);
  draw_mode_list_.push_back(cmd.draw_mode);

  if(vertex_type_ == kNoVertex) {
    vertex_type_ = T::Type();
  } else {
    if(vertex_type_ != T::Type()) {
      SR_ASSERT(!"mesh vbo안에서 vertex type섞어쓰는것은 지원안함");
    }
  }

  SR_CHECK_ERROR("MeshBufferObject Add");
}

}

#endif  // SORA_MESH_H_