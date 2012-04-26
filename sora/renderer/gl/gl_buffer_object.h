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
#ifndef SORA_GL_BUFFER_OBJECT_H_
#define SORA_GL_BUFFER_OBJECT_H_

#include "gl_inc.h"
#include "gl_env.h"

namespace sora {;
namespace gl {
  template<GLenum Target> class GLBufferObject;
  typedef GLBufferObject<GL_ARRAY_BUFFER> GLVertexBufferObject;
  typedef GLBufferObject<GL_ELEMENT_ARRAY_BUFFER> GLIndexBufferObject;

  //버퍼 객체 다루는 정적함수로 구성하자. 정적함수로 구성해야
  //policy 로 꽂아넣기 적절하다(그리고 지저분한 연산이 줄어든다)
  //실제 데이터(핸들)보관은 Handle라는 독립된 클래스를 쓰자
  template<GLenum Target>
  class GLBufferObject {
  public:
    typedef GLuint HandleType;
  public:
    static void Reset(HandleType *handle) {
      *handle = 0;
    }

    static void Init(HandleType *handle, int size, void *data, BufferUsageType usage) {
      GLenum gl_usage = GLEnv::TypeToGLEnum(usage);
      Init(handle, size, data, gl_usage);
    }
    static void Init(HandleType *handle, int size, void *data, GLenum usage) {
      HandleType &buffer = *handle;     
      if(buffer == 0) {
        glGenBuffers(1, &buffer);
      }
      glBindBuffer(Target, buffer);
      glBufferData(Target, size, data, usage);

      //unbind
      glBindBuffer(Target, 0);
    }
    static void Deinit(HandleType *handle) {
      HandleType &buffer = *handle;
      if(buffer != 0) {
        glDeleteBuffers(1, &buffer);
        buffer = 0;
      }
    }
    static bool Loaded(HandleType buffer) {
      return (buffer != 0); 
    }
  };
} //namespace gl
} //namespace sora

#endif  // SORA_GL_BUFFER_OBJECT_H_