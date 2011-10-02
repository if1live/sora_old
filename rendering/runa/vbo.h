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
#ifndef RENDERING_RUNA_VBO_H_
#define RENDERING_RUNA_VBO_H_

namespace runa {;
class VertexBufferObject;
class IndexBufferObject;

class VertexBufferObject {
 public:
  explicit VertexBufferObject(const GLvoid *data,
    int count, GLsizei stride, GLenum usage);
  virtual ~VertexBufferObject();

  inline GLuint buffer() const { return buffer_; }
  inline int count() const { return count_; }
  inline GLsizei stride() const { return stride_; }
 private:
  GLuint buffer_;
  int count_;
  GLsizei stride_;
};

class IndexBufferObject {
 public:
  IndexBufferObject(const GLvoid *data,
    int count, GLenum index_type, GLenum draw_mode, GLenum usage);
  virtual ~IndexBufferObject();

  inline GLuint buffer() const { return buffer_; }
  inline int count() const { return count_; }
  inline GLsizei stride() const { return stride_; }
  inline GLenum index_type() const { return index_type_; }
  inline GLenum draw_mode() const { return draw_mode_; }
 private:
  GLuint buffer_;
  int count_;
  GLsizei stride_;
  GLenum draw_mode_;
  GLenum index_type_;
};
}
#endif  // RENDERING_RUNA_VBO_H_
