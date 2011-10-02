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
#include "precompile.h"

#include "runa/vbo.h"

namespace runa {;
VertexBufferObject::VertexBufferObject(const GLvoid *data,
  int count, GLsizei stride, GLenum usage)
  : buffer_(0), count_(count), stride_(stride) {
  glGenBuffers(1, &buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_);
  glBufferData(GL_ARRAY_BUFFER, stride*count, data, usage);
}
VertexBufferObject::~VertexBufferObject() {
  glDeleteBuffers(1, &buffer_);
}

IndexBufferObject::IndexBufferObject(const GLvoid *data,
  int count, GLenum index_type, GLenum draw_mode, GLenum usage)
  : buffer_(0), count_(count), draw_mode_(draw_mode), index_type_(index_type) {
  // stride 계산
  switch (index_type) {
  case GL_UNSIGNED_BYTE:
    stride_ = sizeof(GLubyte);
    break;
  case GL_UNSIGNED_SHORT:
    stride_ = sizeof(GLushort);
    break;
  case GL_UNSIGNED_INT:
    stride_ = sizeof(GLuint);
    break;
  default:
    SR_ASSERT(!"Not valid index_type");
    break;
  }

  glGenBuffers(1, &buffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, stride_*count, data, usage);
}
IndexBufferObject::~IndexBufferObject() {
  glDeleteBuffers(1, &buffer_);
}
}
