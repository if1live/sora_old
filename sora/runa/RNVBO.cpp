// Ŭnicode please
#include "../SoraStdAfx.h"

#include "runa/RNVBO.h"

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

	IndexBufferObject::IndexBufferObject(const GLvoid *data, int count, GLenum indexType, GLenum drawMode, GLenum usage)
	: buffer_(0), count_(count), drawMode_(drawMode), indexType_(indexType)
	{
		//stride 계산
		switch (indexType) {
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
				MT_ASSERT(!"Not valid indexType");
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