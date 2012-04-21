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
#ifndef SORA_VERTEX_H_
#define SORA_VERTEX_H_

#include "core/vector.h"

namespace sora {;

typedef enum {
  kNoVertex = -1,
  kVertex2D,
  kVertex,
  kTangentVertex
} VertexType;

struct Vertex;
struct TangentVertex;
struct Vertex2D;
typedef std::vector<Vertex> VertexList;
typedef std::vector<TangentVertex> TangentVertexList;
typedef std::vector<Vertex2D> Vertex2DList;
typedef std::vector<unsigned short> IndexList;

struct Vertex2D {
  Vertex2D() : pos(0, 0), texcoord(0, 0) {}
  Vertex2D(float x, float y, float s, float t)
    : pos(x, y), texcoord(s, t) {}
  glm::vec2 pos;
  glm::vec2 texcoord;
  static VertexType Type() { return kVertex2D; }
};

struct Vertex {
  Vertex() : pos(0, 0, 0), texcoord(0, 0), normal(1, 0, 0), color(255, 255, 255, 255) {}
  Vertex(const glm::vec3 &pos, const glm::vec2 &texcoord)
    : pos(pos), texcoord(texcoord), normal(1, 0, 0), color(255, 255, 255, 255) {}
  Vertex(const glm::vec3 &pos)
    : pos(pos), texcoord(0, 0), normal(1, 0, 0), color(255, 255, 255, 255) {}

  glm::vec3 pos;
  glm::vec2 texcoord;
  glm::vec3 normal;
  sora::vec4ub color;

  static VertexType Type() { return kVertex; }
};

struct TangentVertex : public Vertex {
  glm::vec3 tangent;
  static VertexType Type() { return kTangentVertex; }
};

}

#endif  // SORA_VERTEX_H_