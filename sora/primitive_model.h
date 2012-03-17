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

namespace sora {;
class SR_DLL PrimitiveModel {
public:
  PrimitiveModel(std::vector<Vertex> *vert_list, std::vector<ushort> *index_list);
  ~PrimitiveModel();
  //use GL_TRIANGLES
  void WireCube(float width, float height, float depth);
  void SolidCube(float width, float height, float depth);
  void WireSphere(float radius, int slices, int stacks);
  void SolidSphere(float radius, int slices, int stacks);
  void WireCone(float base, float height, int slices, int stacks);
  void SolidCone(float base, float height, int slices, int stacks);
  void WireCylinder(float radius, float height, int slices);
  void SolidCylinder(float radius, float height, int slices);
  void WireAxis(float size);

private:
  std::vector<Vertex> *vert_list_;
  std::vector<ushort> *index_list_;
};

}

#endif  // SORA_PRIMITIVE_MODEL_H_