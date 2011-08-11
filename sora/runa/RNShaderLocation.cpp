// Ŭnicode please
#include "../SoraStdAfx.h"
#include "runa/RNShaderLocation.h"

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
	const std::string &ShaderLocation::getName() const 
	{
		return name_; 
	}
	GLint ShaderLocation::getLocation() const 
	{
		return location_;
	}
	GLint ShaderLocation::getSize() const 
	{
		return size_; 
	}
	GLenum ShaderLocation::getType() const 
	{
		return type_; 
	}

	std::string ShaderLocation::str() const
	{
		std::ostringstream oss;
		oss << "[Uniform]" << getLocation() << "/" << getName();
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