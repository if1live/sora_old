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
#if 0
#ifndef SORA_PRIMITIVE_MODEL_H_
#define SORA_PRIMITIVE_MODEL_H_

#include "renderer/vertex.h"
#include "renderer/mesh.h"
#if SR_USE_PCH == 0
#include <vector>
#include "gl_inc.h"
#endif

namespace sora {;
struct PrimitiveModelImpl;

typedef std::vector<Vertex> VertexListType;
typedef std::vector<ushort> IndexListType;


class PrimitiveModel {
public:

  PrimitiveModel();
  ~PrimitiveModel();
  PrimitiveModel(const PrimitiveModel &o);
  PrimitiveModel& operator=(const PrimitiveModel &o);
  
  void SolidCube(float width, float height, float depth);

  void SolidSphere(float radius, int slices, int stacks);

  void SolidCone(float base, float height, int slices, int stacks);

  void SolidCylinder(float radius, float height, int slices);

  void SolidTeapot(float size);

public:
  void SolidPlane(float half_size);

  template<typename VertexType>
  std::vector< DrawCommand<VertexType> > GetDrawCmdList() const;

private:
  // 몇개의 submodel로 구성되는가
  int Count() const;
  const Vertex* vertex_list(int idx) const;
  const void *index_list(int idx) const;
  int vertex_count(int idx) const;
  int index_count(int idx) const;
  GLenum draw_mode(int idx) const;
};

template<typename VertexType>
std::vector< DrawCommand<VertexType> > PrimitiveModel::GetDrawCmdList() const {
  std::vector< DrawCommand<VertexType> > cmd_list;
  //draw primitive model
  for(int i = 0 ; i < Count() ; i++) {
    DrawCommand draw_cmd;
    draw_cmd.vert_ptr = vertex_list(i);
    draw_cmd.index_ptr = index_list(i);
    draw_cmd.index_count = index_count(i);
    draw_cmd.vert_count = vertex_count(i);
    draw_cmd.draw_mode = draw_mode(i);
    draw_cmd.index_type = GL_UNSIGNED_SHORT;

    cmd_list.push_back(draw_cmd);
  }
  return cmd_list;
}
}

#endif  // SORA_PRIMITIVE_MODEL_H_
#endif