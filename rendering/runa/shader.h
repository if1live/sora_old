// Ŭnicode please
#pragma once

#include "runa/runa_enum.h"

namespace runa
{
	template<GLenum ShaderType>
	class Shader {
	public:
		enum {
			Type = ShaderType,
		};
	public:
		Shader();
		Shader(const std::string &src);
		~Shader();

		bool Load(const std::string &src);

		GLuint get_handle() const;

	private:
		GLuint handle_;
	};
}
#include "runa/shader.hpp"