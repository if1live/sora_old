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
#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

namespace sora {;
struct Vertex;
struct TangentVertex;
struct Vertex2D;

struct Vertex2D {
  enum {
    kPosType = GL_FLOAT,
    kTexcoordType = GL_FLOAT,
  };
  Vertex2D() : pos(0, 0), texcoord(0, 0) {}
  Vertex2D(float x, float y, float s, float t)
    : pos(x, y), texcoord(s, t) {}

  glm::vec2 pos;
  glm::vec2 texcoord;
};

struct Vertex {
  enum {
    kPosType = GL_FLOAT,
    kTexcoordType = GL_FLOAT,
    kNormalType = GL_FLOAT,
    kColorType = GL_UNSIGNED_BYTE,
  };
  Vertex() : pos(0, 0, 0), texcoord(0, 0), normal(1, 0, 0), color(1, 1, 1, 1) {}
  Vertex(float x, float y, float z, float s, float t, float nx, float ny, float nz, uchar r, uchar g, uchar b, uchar a)
    : pos(x, y, z), texcoord(s, t), normal(nx, ny, nz), color(r, g, b, a) {}
  glm::vec3 pos;
  glm::vec2 texcoord;
  glm::vec3 normal;
  sora::vec4ub color;
};
/*
struct TangentVertex {
  vec3 pos;
  vec2 texcoord;
  vec3 normal;
  
  vec3 tangent;
  vec3 binormal;
};
*/
}

#endif  // SORA_VERTEX_H_