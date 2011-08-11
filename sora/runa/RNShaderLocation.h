// Ŭnicode please
#pragma once

namespace runa
{
	class ShaderLocation {
	public:
		ShaderLocation(const char *name, GLint location, GLint size, GLenum type);

		const std::string &getName() const;
		GLint getLocation() const;
		GLint getSize() const;
		GLenum getType() const;

		bool operator==(const ShaderLocation &o) const;
		bool operator!=(const ShaderLocation &o) const;

		std::string str() const;

	private:
		std::string name_;
		GLint location_;
		GLint size_;
		GLenum type_;
	};
}