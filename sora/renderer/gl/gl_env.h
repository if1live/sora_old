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
#ifndef SORA_GL_HELPER_H_
#define SORA_GL_HELPER_H_

#include <vector>
#include <string>
#include "renderer/globals.h"
#include "core/vertex.h"

namespace sora {;
namespace gl {
  class GLEnv {
  public:
    static bool CheckFrameBufferStatus(const char *name);
    static bool CheckFrameBufferStatus(const std::string &name) {
      return CheckFrameBufferStatus(name.c_str());
    }
    static bool CheckError(const char *name);
    static bool CheckError(const std::string &name) {
      return CheckError(name.c_str());
    }

    //템플릿에서 사용되는 인터페이스측
  public:  
    // gl information
    const std::string &GetVersion() const;
    const std::string &GetVender() const;
    const std::string &GetRenderer() const;
    const std::vector<std::string> &GetExtensionList() const;

    //gl에서만 쓰이는 부분
  public:
    static GLenum TypeToGLEnum(DrawType type);
    static GLenum TypeToGLEnum(TexFormatType type);
    static GLenum TypeToGLEnum(TexMagFilter type);
    static GLenum TypeToGLEnum(TexMinFilter type);
    static GLenum TypeToGLEnum(TexWrapMode type);
    static GLenum TypeToGLEnum(BufferUsageType type);

    //vertex 가공용
  public:
    static GLenum VertexElemTypeToGLEnum(VertexElemType type);
    static VertexInfo ToGLVertexInfo(const VertexInfo &info);
    template<typename T>
    static VertexInfo &GetGLVertexInfo() {
      static bool run = false;
      static VertexInfo info;
      if(run == false) {
        run = true;
        info = ToGLVertexInfo(VertexInfoHolder<T>::Get());
      }
      return info;
    }
  };
  

  template<typename T>
  struct TypeToEnum { };
  template<>  struct TypeToEnum<float> { enum { value = GL_FLOAT }; };
  template<>  struct TypeToEnum<int> { enum { value = GL_INT }; };
  template<>  struct TypeToEnum<unsigned int> { enum { value = GL_UNSIGNED_INT }; };
  template<>  struct TypeToEnum<char> { enum { value = GL_BYTE }; };
  template<>  struct TypeToEnum<unsigned char> { enum { value = GL_UNSIGNED_BYTE }; };
  template<>  struct TypeToEnum<short> { enum { value = GL_SHORT }; };
  template<>  struct TypeToEnum<unsigned short> { enum { value = GL_UNSIGNED_SHORT }; };

  template<typename T>
  struct VecToGLEnum {
    enum {
      value = TypeToEnum<typename T::value_type>::value
    };
  };
} //namespace gl
} //namespace sora

#endif  // SORA_GL_HELPER_H_