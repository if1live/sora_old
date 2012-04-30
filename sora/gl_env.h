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
#include "globals.h"
#include "vertex.h"

namespace sora {;
namespace gl {
  typedef GLint ShaderVariableHandleType;
  typedef GLuint ShaderHandleType;
  typedef GLuint TextureHandleType;

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
    static const VertexInfo &GetGLVertexInfo() {
      static bool run = false;
      static VertexInfo info;
      if(run == false) {
        run = true;
        info = ToGLVertexInfo(VertexInfoHolder<T>::Get());
      }
      return info;
    }
    static const VertexInfo &GetGLVertexInfo(VertexCode code);

    static VarType GLEnumToVarType(GLenum type);
    static GLenum VarTypeToGLEnum(VarType type);
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

  template<VarType N> struct VarTypeToEnum {};
  template<> struct VarTypeToEnum<kTypeFloat> { enum { value = GL_FLOAT }; };
  template<> struct VarTypeToEnum<kTypeInt> { enum { value = GL_INT }; };
  template<> struct VarTypeToEnum<kTypeUint> { enum { value = GL_UNSIGNED_INT }; };
  template<> struct VarTypeToEnum<kTypeShort> { enum { value = GL_SHORT }; };
  template<> struct VarTypeToEnum<kTypeUshort> { enum { value = GL_UNSIGNED_SHORT }; };
  template<> struct VarTypeToEnum<kTypeByte> { enum { value = GL_BYTE }; };
  template<> struct VarTypeToEnum<kTypeUbyte> { enum { value = GL_UNSIGNED_BYTE }; };

  template<> struct VarTypeToEnum<kTypeFloatMat4> { enum { value = GL_FLOAT_MAT4 }; };
  template<> struct VarTypeToEnum<kTypeFloatMat3> { enum { value = GL_FLOAT_MAT3 }; };
  template<> struct VarTypeToEnum<kTypeFloatMat2> { enum { value = GL_FLOAT_MAT2 }; };

  template<> struct VarTypeToEnum<kTypeFloatVec4> { enum { value = GL_FLOAT_VEC4 }; };
  template<> struct VarTypeToEnum<kTypeFloatVec3> { enum { value = GL_FLOAT_VEC3 }; };
  template<> struct VarTypeToEnum<kTypeFloatVec2> { enum { value = GL_FLOAT_VEC2 }; };
  template<> struct VarTypeToEnum<kTypeIntVec4> { enum { value = GL_INT_VEC4 }; };
  template<> struct VarTypeToEnum<kTypeIntVec3> { enum { value = GL_INT_VEC3 }; };
  template<> struct VarTypeToEnum<kTypeIntVec2> { enum { value = GL_INT_VEC2 }; };

  template<> struct VarTypeToEnum<kTypeSample2D> { enum { value = GL_SAMPLER_2D }; };
  template<> struct VarTypeToEnum<kTypeSampleCube> { enum { value = GL_SAMPLER_CUBE }; };
} //namespace gl
} //namespace sora

#endif  // SORA_GL_HELPER_H_