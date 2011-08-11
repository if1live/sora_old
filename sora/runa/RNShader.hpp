// Ŭnicode please
#pragma once

namespace runa
{
	template<GLenum shaderType>
	Shader<shaderType>::Shader()
		: handle_(0)
	{
	}
	template<GLenum shaderType>
	Shader<shaderType>::Shader(const std::string &src)
		: handle_(0)
	{
		load(src);
	}

	template<GLenum shaderType>
	bool Shader<shaderType>::load(const std::string &src)
	{
		handle_ = glCreateShader(shaderType);
		const char *srcPtr = src.c_str();
		glShaderSource(handle_, 1, &srcPtr, 0);
		glCompileShader(handle_);

		GLint status;
		glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE)
		{
			GLchar msg[1024];
			glGetShaderInfoLog(handle_, sizeof(msg), 0, &msg[0]);
			const char *srccode = src.c_str();
			MT_LINE_LOG(srccode);
			MT_LINE_LOG(msg);
			MT_ASSERT(!msg);
			return false;
		}
		return true;
	}

	template<GLenum shaderType>
	Shader<shaderType>::~Shader()
	{
		if(handle_ > 0) 
		{
			glDeleteShader(handle_);
			handle_ = 0;
		}
	}

	template<GLenum shaderType>
	GLuint Shader<shaderType>::getHandle() const 
	{
		return handle_; 
	}
}