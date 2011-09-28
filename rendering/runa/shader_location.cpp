// Ŭnicode please
#include "precompile.h"
#include "runa/shader_location.h"

using namespace std;

namespace runa
{
	ShaderLocation::ShaderLocation(const char *name, GLint location, GLint size, GLenum type)
		: name_(name),
		location_(location), 
		size_(size),
		type_(type)
	{
	}
	std::string ShaderLocation::str() const
	{
		std::ostringstream oss;
		oss << "[Uniform]" << location() << "/" << name();
		return oss.str();
	}

	bool ShaderLocation::operator==(const ShaderLocation &o) const
	{
		return (name_ == o.name_ && location_ == o.location_
			&& size_ == o.size_ && type_ == o.type_);
	}
	bool ShaderLocation::operator!=(const ShaderLocation &o) const
	{
		return !(*this == o);
	}
}