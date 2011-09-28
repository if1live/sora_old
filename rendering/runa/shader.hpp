// Ŭnicode please
#pragma once

namespace runa
{
	template<GLenum ShaderType>
	Shader<ShaderType>::Shader()
		: handle_(0)
	{
	}
	template<GLenum ShaderType>
	Shader<ShaderType>::Shader(const std::string &src)
		: handle_(0)
	{
		Load(src);
	}

	template<GLenum ShaderType>
	bool Shader<ShaderType>::Load(const std::string &src)
	{
		handle_ = glCreateShader(ShaderType);
		const char *srcPtr = src.c_str();
		glShaderSource(handle_, 1, &srcPtr, 0);
		glCompileShader(handle_);

		GLint status;
		glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			GLchar msg[1024];
			glGetShaderInfoLog(handle_, sizeof(msg), 0, &msg[0]);
			const char *srccode = src.c_str();
			MT_LINE_LOG(srccode);
			MT_LINE_LOG(msg);
			SR_ASSERT(!msg);
			return false;
		}
		return true;
	}

	template<GLenum ShaderType>
	Shader<ShaderType>::~Shader()
	{
		if(handle_ > 0) {
			glDeleteShader(handle_);
			handle_ = 0;
		}
	}

	template<GLenum ShaderType>
	GLuint Shader<ShaderType>::handle() const 
	{
		return handle_; 
	}
}