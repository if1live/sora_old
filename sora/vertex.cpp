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
#include "vertex.h"

namespace sora {;
Vertex CreateVertex(const glm::vec3 &pos, const glm::vec2 &texcoord) {
  Vertex vert;
  vert.set_pos(pos);
  vert.set_texcoord(texcoord);
  return vert;
}

Vertex2D CreateVertex2D(float x, float y, float s, float t) {
  Vertex2D vert;
  vert.pos[0] = x;
  vert.pos[1] = y;
  vert.texcoord[0] = s;
  vert.texcoord[1] = t;
  return vert;
}

VertexInfo::VertexInfo()
  : vert_code(kVertexNone),
  size(0),
  pos_offset(-1),
  pos_type(kTypeFloat),
  pos_dim(0),
  color_offset(-1),
  color_type(kTypeFloat),
  color_dim(0),
  texcoord_offset(-1),
  texcoord_type(kTypeFloat),
  texcoord_dim(0),
  normal_offset(-1),
  normal_type(kTypeFloat),
  normal_dim(0),
  tangent_offset(-1),
  tangent_type(kTypeFloat),
  tangent_dim(0) { }

const VertexInfo &VertexInfo::Info(VertexCode code) {
  static bool run = false;
  static std::array<VertexInfo, kVertexCodeCount> info_list;
  if(run == false) {
    run = true;

    info_list[kVertex2D] = VertexInfoHolder<Vertex2D>::Get();
    info_list[kVertex] = VertexInfoHolder<Vertex>::Get();
    info_list[kVertexTangent] = VertexInfoHolder<TangentVertex>::Get();
    info_list[kVertexPos2D] = VertexInfoHolder<glm::vec2>::Get();
    info_list[kVertexPos3D] = VertexInfoHolder<glm::vec3>::Get();
  }
  return info_list[code];
}
bool VertexInfo::operator==(const VertexInfo &o) const {
  if(memcmp(this, &o, sizeof(VertexInfo)) == 0) {
    return true;
  } else {
    return false;
  }
}
bool VertexInfo::operator!=(const VertexInfo &o) const {
  return !(*this == o);
}

} //namespace sora

