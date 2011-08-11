// Ŭnicode please
#pragma once

namespace runa
{
	class VertexBufferObject;
	class IndexBufferObject;

	class VertexBufferObject {
	public:
		VertexBufferObject(const GLvoid *data, int count, GLsizei stride, GLenum usage);
		virtual ~VertexBufferObject();

		inline GLuint getBuffer() const { return buffer_; }
		inline int getCount() const { return count_; }
		inline GLsizei getStride() const { return stride_; }
	private:
		GLuint buffer_;
		int count_;
		GLsizei stride_;
	};

	class IndexBufferObject {
	public:
		IndexBufferObject(const GLvoid *data, int count, GLenum indexType, GLenum drawMode, GLenum usage);
		virtual ~IndexBufferObject();

		inline GLuint getBuffer() const { return buffer_; }
		inline int getCount() const { return count_; }
		inline GLsizei getStride() const { return stride_; }
		inline GLenum getIndexType() const { return indexType_; }
		inline GLenum getDrawMode() const { return drawMode_; }
	private:
		GLuint buffer_;
		int count_;
		GLsizei stride_;
		GLenum drawMode_;
		GLenum indexType_;
	};

}