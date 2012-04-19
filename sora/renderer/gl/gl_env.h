/*  Copyright (C) 2011 by if1live */
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
    template<typename T>  static GLenum TypeToGLEnum() { 
      static_assert(false, "no type");
      return GL_FLOAT; 
    }
    template<> static GLenum TypeToGLEnum<float>() { return GL_FLOAT; }
    template<> static GLenum TypeToGLEnum<int>() { return GL_INT; }
    template<> static GLenum TypeToGLEnum<unsigned int>() { return GL_UNSIGNED_INT; }
    template<> static GLenum TypeToGLEnum<char>() { return GL_BYTE; }
    template<> static GLenum TypeToGLEnum<unsigned char>() { return GL_UNSIGNED_BYTE; }
    template<> static GLenum TypeToGLEnum<short>() { return GL_SHORT; }
    template<> static GLenum TypeToGLEnum<unsigned short>() { return GL_UNSIGNED_SHORT; }

    template<typename T>
    static GLenum VecToGLEnum(const T &value) {
      return TypeToGLEnum<typename T::value_type>();
    }
  };
} //namespace gl
} //namespace sora

#endif  // SORA_GL_HELPER_H_