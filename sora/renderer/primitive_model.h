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
#ifndef SORA_PRIMITIVE_MODEL_H_
#define SORA_PRIMITIVE_MODEL_H_

#include "vertex.h"
#include "mesh.h"
#if SR_USE_PCH == 0
#include <vector>
#include "gl_inc.h"
#endif

namespace sora {;
struct PrimitiveModelImpl;

typedef std::vector<Vertex> VertexListType;
typedef std::vector<ushort> IndexListType;

class SR_DLL PrimitiveModel {
public:
  PrimitiveModel();
  ~PrimitiveModel();
  PrimitiveModel(const PrimitiveModel &o);
  PrimitiveModel& operator=(const PrimitiveModel &o);
  //use GL_TRIANGLES
  void WireCube(float width, float height, float depth);

  //discreate normal : true=>면단위로 noraml확정, false=>구같은 느낌의 노멀벡터 사용
  void SolidCube(float width, float height, float depth, bool discreate_normal);
  void WireSphere(float radius, int slices, int stacks);
  void SolidSphere(float radius, int slices, int stacks);
  void WireCone(float base, float height, int slices, int stacks);
  void SolidCone(float base, float height, int slices, int stacks);
  void WireCylinder(float radius, float height, int slices);
  void SolidCylinder(float radius, float height, int slices);
  void WireAxis(float size);
  void WireTeapot(float size);
  void SolidTeapot(float size);

  std::vector<DrawCommand> GetDrawCmdList_wire() const;
  std::vector<DrawCommand> GetDrawCmdList_solid() const;
  std::vector<DrawCommand> GetDrawCmdList() const;

private:
  // 몇개의 submodel로 구성되는가
  int Count() const;
  const Vertex* vertex_list(int idx) const;
  const void *index_list(int idx) const;
  int vertex_count(int idx) const;
  int index_count(int idx) const;
  GLenum draw_mode(int idx) const;

  PrimitiveModelImpl *impl;

  static void CalcTeapotSize();
  //teapot
  static float teapot_min_x_;
  static float teapot_max_x_;
  static float teapot_min_y_;
  static float teapot_max_y_;
  static float teapot_min_z_;
  static float teapot_max_z_;
  static bool teapot_created_;
};

}

#endif  // SORA_PRIMITIVE_MODEL_H_