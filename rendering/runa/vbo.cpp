// Ŭnicode please
#include "precompile.h"

#include "runa/vbo.h"

namespace runa
{
  VertexBufferObject::VertexBufferObject(const GLvoid *data, int count, GLsizei stride, GLenum usage)
    : buffer_(0), count_(count), stride_(stride)
  {
    glGenBuffers(1, &buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, stride*count, data, usage);

  }
  VertexBufferObject::~VertexBufferObject()
  {
    glDeleteBuffers(1, &buffer_);
  }

  IndexBufferObject::IndexBufferObject(const GLvoid *data, int count, GLenum index_type, GLenum draw_mode, GLenum usage)
    : buffer_(0), count_(count), draw_mode_(draw_mode), index_type_(index_type)
  {
    //stride 계산
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
  IndexBufferObject::~IndexBufferObject()
  {
    glDeleteBuffers(1, &buffer_);
  }
}