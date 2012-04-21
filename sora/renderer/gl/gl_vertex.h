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
#ifndef SORA_GL_VERTEX_H_
#define SORA_GL_VERTEX_H_

#include "core/vector.h"
#include "core/vertex.h"
#include "core/template_lib.h"
#include "gl_env.h"

namespace sora {;
namespace gl {
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

  template<typename T>
  struct VertexInfoHolder {
    typedef T VertexType;
    static VertexInfo &Get(Type2Type<T> t) {
      static_assert(false, "not defined");
      static VertexInfo info;
      return info;
    }
  };

  template<>
  struct VertexInfoHolder<Vertex2D> {
    typedef Vertex2D VertexType;
    static VertexInfo &Get() {
      static VertexInfo info;
      static bool init = false;
      if(init == false) {
        VertexType vert;

        init = true;
        info.size = sizeof(vert);

        info.pos_offset = offsetof(VertexType, pos);
        info.pos_type = VecToGLEnum<decltype(vert.pos)>::value;

        info.texcoord_offset = offsetof(VertexType, texcoord);
        info.texcoord_type = VecToGLEnum<decltype(vert.texcoord)>::value;

        info.color_offset = -1;
        info.normal_offset = -1;
        info.tangent_offset = -1;
      }
      return info;
    }
  };

  template<>
  struct VertexInfoHolder<Vertex> {
    typedef Vertex VertexType;
    static VertexInfo &Get() {
      static VertexInfo info;
      static bool init = false;
      if(init == false) {
        init = true;
        VertexType vert;
        info.size = sizeof(vert);

        info.pos_offset = offsetof(VertexType, pos);
        info.pos_type = VecToGLEnum<decltype(vert.pos)>::value;

        info.texcoord_offset = offsetof(VertexType, texcoord);
        info.texcoord_type = VecToGLEnum<decltype(vert.texcoord)>::value;

        info.color_offset = offsetof(VertexType, color);
        info.color_type = VecToGLEnum<decltype(vert.color)>::value;

        info.normal_offset = offsetof(VertexType, normal);
        info.normal_type = VecToGLEnum<decltype(vert.normal)>::value;

        info.tangent_offset = -1;
      }
      return info;
    }
  };

  template<>
  struct VertexInfoHolder<TangentVertex> {
    typedef TangentVertex VertexType;
    static VertexInfo &Get() {
      static VertexInfo info;
      static bool init = false;
      if(init == false) {
        init = true;
        VertexType vert;
        info.size = sizeof(vert);

        info.pos_offset = offsetof(VertexType, pos);
        info.pos_type = VecToGLEnum<decltype(vert.pos)>::value;

        info.texcoord_offset = offsetof(VertexType, texcoord);
        info.texcoord_type = VecToGLEnum<decltype(vert.texcoord)>::value;

        info.color_offset = offsetof(VertexType, color);
        info.color_type = VecToGLEnum<decltype(vert.color)>::value;

        info.normal_offset = offsetof(VertexType, normal);
        info.normal_type = VecToGLEnum<decltype(vert.normal)>::value;

        info.tangent_offset = offsetof(VertexType, tangent);
        info.tangent_type = VecToGLEnum<decltype(vert.tangent)>::value;
      }
      return info;
    }
  };
}
}

#endif  // SORA_GL_VERTEX_H_