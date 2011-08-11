// Ŭnicode please
#pragma once

#include <vector>
#include <memory>
#if _IPHONE_
#include <tr1/memory>
#endif

#include "runa/RNTypedef.h"

namespace runa
{
	template<GLenum shaderType>
	class Shader {
	public:
		enum {
			Type = shaderType,
		};
	public:
		Shader();
		Shader(const std::string &src);
		~Shader();

		bool load(const std::string &src);

		GLuint getHandle() const;

	private:
		GLuint handle_;
	};
}
#include "runa/RNShader.hpp"