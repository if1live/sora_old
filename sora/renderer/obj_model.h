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
#ifndef SORA_OBJ_MODEL_H_
#define SORA_OBJ_MODEL_H_

#include "renderer/vertex.h"
#include "mesh.h"
#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;

struct ObjModel;

struct SR_DLL ObjModel {
  ObjModel();
  ~ObjModel();
  ObjModel(const ObjModel &o);
  ObjModel& operator=(const ObjModel &o);

  const Vertex *vertex_ptr() const;
  const void *index_ptr() const;
  int vertex_count() const;
  int index_count() const;

  std::vector<DrawCommand<Vertex>> GetDrawCmdList_wire() const;

  template<typename VertexType>
  std::vector< DrawCommand<VertexType> > GetDrawCmdList_solid() const;

  void AddVertex(const Vertex &v);
  void AddIndex(ushort idx);

private:
  std::vector<Vertex> vert_list_;
  // GL_TRIANGLES로 그릴수 잇도록 정렬된 인덱스 리스트
  std::vector<ushort> index_list_;
  // GL_LINES로 그릴수 잇도록 정렬된거. 거의 테스트용으로 쓰일테니까 최적화는 안함
  mutable std::vector<ushort> line_index_list_;
};

template<typename VertexType>
std::vector< DrawCommand<VertexType> > ObjModel::GetDrawCmdList_solid() const {
  std::vector< DrawCommand<VertexType> > cmd_list;
  DrawCommand draw_cmd;
  draw_cmd.vert_ptr = vertex_ptr();
  draw_cmd.draw_mode = GL_TRIANGLES;
  draw_cmd.index_ptr = index_ptr();
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.index_count = index_count();
  draw_cmd.vert_count = vertex_count();
  cmd_list.push_back(draw_cmd);
  return cmd_list;
}
}

#endif  // SORA_OBJ_MODEL_H_