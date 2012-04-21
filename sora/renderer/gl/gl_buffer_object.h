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

  template<GLenum Target>
  class GLBufferObject {
  public:
    GLBufferObject();
    ~GLBufferObject();

    void Init(int size, void *data, BufferUsageType usage);
    void Init(int size, void *data, GLenum usage);
    void Deinit();
    bool Loaded() const { return (buffer_ != 0); }

    GLuint buffer() const { return buffer_; }
  private:
    GLuint buffer_;
  };

  template<GLenum Target>
  GLBufferObject<Target>::GLBufferObject() 
    : buffer_(0) {
  }

  template<GLenum Target>
  GLBufferObject<Target>::~GLBufferObject() {
  }

  template<GLenum Target>
  void GLBufferObject<Target>::Init(int size, void *data, BufferUsageType usage) {
    GLenum gl_usage = GLEnv::TypeToGLEnum(usage);
    Init(size, data, gl_usage);
  }
  template<GLenum Target>
  void GLBufferObject<Target>::Init(int size, void *data, GLenum usage) {
    if(buffer_ == 0) {
      glGenBuffers(1, &buffer_);
    }
    glBindBuffer(Target, buffer_);
    glBufferData(Target, size, data, usage);

    //unbind
    glBindBuffer(Target, 0);
  }

  template<GLenum Target>
  void GLBufferObject<Target>::Deinit() {
    if(buffer_ != 0) {
      glDeleteBuffers(1, &buffer_);
      buffer_ = 0;
    }
  }

} //namespace gl
} //namespace sora

#endif  // SORA_GL_BUFFER_OBJECT_H_