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
		IndexBufferObject(const GLvoid *data, int count, GLenum index_type, GLenum draw_mode, GLenum usage);
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