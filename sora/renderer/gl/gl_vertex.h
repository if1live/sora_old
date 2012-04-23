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
    VertexInfo()
    : size(0),
    pos_offset(-1),
    pos_type(GL_FLOAT),
    pos_dim(0),
    color_offset(-1),
    color_type(GL_FLOAT),
    color_dim(0),
    texcoord_offset(-1),
    texcoord_type(GL_FLOAT),
    texcoord_dim(0),
    normal_offset(-1),
    normal_type(GL_FLOAT),
    normal_dim(0),
    tangent_offset(-1),
    tangent_type(GL_FLOAT),
    tangent_dim(0) { }

    int size;

    int pos_offset;
    GLenum pos_type;
    int pos_dim;

    int color_offset;
    GLenum color_type;
    int color_dim;

    int texcoord_offset;
    GLenum texcoord_type;
    int texcoord_dim;

    int normal_offset;
    GLenum normal_type;
    int normal_dim;

    int tangent_offset;
    GLenum tangent_type;
    int tangent_dim;
  };

  //표준형 vertex에 대한 구현
  template<typename T>
  struct VertexInfoHolder {
    typedef T VertexType;
    static VertexInfo &Get() {
      static VertexInfo info;
      static bool init = false;
      if(init == false) {
        init = true;
        typedef T VertexType;
        VertexType vert;
        info.size = sizeof(T);

        info.pos_dim = T::PosDim;
        if(T::PosDim > 0) {
          info.pos_offset = offsetof(VertexType, pos);
          info.pos_type = VecToGLEnum<decltype(vert.pos)>::value;
        }
        info.texcoord_dim = T::TexcoordDim;
        if(T::TexcoordDim > 0) {
          info.texcoord_offset = offsetof(VertexType, texcoord);
          info.texcoord_type = VecToGLEnum<decltype(vert.texcoord)>::value;
        }
        info.color_dim = T::ColorDim;
        if(T::ColorDim > 0) {
          info.color_offset = offsetof(VertexType, color);
          info.color_type = VecToGLEnum<decltype(vert.color)>::value;
        }
        info.normal_dim = T::NormalDim;
        if(T::NormalDim > 0) {
          info.normal_offset = offsetof(VertexType, normal);
          info.normal_type = VecToGLEnum<decltype(vert.normal)>::value;
        }
        info.tangent_dim = T::TangentDim;
        if(T::TangentDim > 0) {
          info.tangent_offset = offsetof(VertexType, tangent);
          info.tangent_type = VecToGLEnum<decltype(vert.tangent)>::value;    
        }
      }
      return info;
    }
  };

  template<>
  struct VertexInfoHolder<glm::vec2> {
    typedef glm::vec2 VertexType;
    static VertexInfo &Get() {
      static VertexInfo info;
      static bool init = false;
      if(init == false) {
        init = true;
        info.size = sizeof(glm::vec2);

        info.pos_offset = 0;
        info.pos_type = TypeToEnum<float>::value;
        info.pos_dim = 2;
      }
      return info;
    }
  };

  template<>
  struct VertexInfoHolder<glm::vec3> {
    typedef glm::vec3 VertexType;
    static VertexInfo &Get() {
      static VertexInfo info;
      static bool init = false;
      if(init == false) {
        init = true;
        info.size = sizeof(glm::vec3);

        info.pos_offset = 0;
        info.pos_type = TypeToEnum<float>::value;
        info.pos_dim = 3;
      }
      return info;
    }
  };
}
}

#endif  // SORA_GL_VERTEX_H_