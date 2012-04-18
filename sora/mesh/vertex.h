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

enum {
  kNoVertex = -1,
  kVertex2D,
  kVertex,
  kTangentVertex
};

struct VertexInfo {
  int size;

  int pos_offset;
  GLenum pos_type;

  int color_offset;
  GLenum color_type;

  int texcoord_offset;
  GLenum texcoord_type;

  int normal_offset;
  GLenum normal_type;

  int tangent_offset;
  GLenum tangent_type;
};

struct Vertex2D {
  Vertex2D() : pos(0, 0), texcoord(0, 0) {}
  Vertex2D(float x, float y, float s, float t)
    : pos(x, y), texcoord(s, t) {}

  glm::vec2 pos;
  glm::vec2 texcoord;

  static int Type() { return kVertex2D; }
  static VertexInfo &Info() {
    static VertexInfo info;
    static bool init = false;
    if(init == false) {
      Vertex2D vert;

      init = true;
      info.size = sizeof(Vertex2D);

      info.pos_offset = offsetof(Vertex2D, pos);
      info.pos_type = GLEnv::VecToGLEnum(vert.pos);

      info.texcoord_offset = offsetof(Vertex2D, texcoord);
      info.texcoord_type = GLEnv::VecToGLEnum(vert.texcoord);

      info.color_offset = -1;
      info.normal_offset = -1;
      info.tangent_offset = -1;
    }
    return info;
  }
};

struct Vertex {
  Vertex() : pos(0, 0, 0), texcoord(0, 0), normal(1, 0, 0), color(255, 255, 255, 255) {}

  glm::vec3 pos;
  glm::vec2 texcoord;
  glm::vec3 normal;
  sora::vec4ub color;

  static int Type() { return kVertex; }
  static VertexInfo &Info() {
    static VertexInfo info;
    static bool init = false;
    if(init == false) {
      init = true;
      Vertex vert;
      info.size = sizeof(Vertex);

      info.pos_offset = offsetof(Vertex, pos);
      info.pos_type = GLEnv::VecToGLEnum(vert.pos);

      info.texcoord_offset = offsetof(Vertex, texcoord);
      info.texcoord_type = GLEnv::VecToGLEnum(vert.texcoord);

      info.color_offset = offsetof(Vertex, color);
      info.color_type = GLEnv::VecToGLEnum(vert.color);

      info.normal_offset = offsetof(Vertex, normal);
      info.normal_type = GLEnv::VecToGLEnum(vert.normal);

      info.tangent_offset = -1;
    }
    return info;
  }
};

struct TangentVertex : public Vertex {
  glm::vec3 tangent;

  static int Type() { return kTangentVertex; }
  static VertexInfo &Info() {
    static VertexInfo info;
    static bool init = false;
    if(init == false) {
      init = true;
      TangentVertex vert;
      info.size = sizeof(TangentVertex);

      info.pos_offset = offsetof(TangentVertex, pos);
      info.pos_type = GLEnv::VecToGLEnum(vert.pos);

      info.texcoord_offset = offsetof(TangentVertex, texcoord);
      info.texcoord_type = GLEnv::VecToGLEnum(vert.texcoord);

      info.color_offset = offsetof(TangentVertex, color);
      info.color_type = GLEnv::VecToGLEnum(vert.color);

      info.normal_offset = offsetof(TangentVertex, normal);
      info.normal_type = GLEnv::VecToGLEnum(vert.normal);

      info.tangent_offset = offsetof(TangentVertex, tangent);
      info.tangent_type = GLEnv::VecToGLEnum(vert.tangent);
    }
    return info;
  }
};
}

#endif  // SORA_VERTEX_H_