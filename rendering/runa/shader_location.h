// Ŭnicode please
#pragma once

#include "sora/macro.h"

namespace runa
{
  class ShaderLocation {
  public:
    ShaderLocation(const char *name, GLint location, GLint size, GLenum type);

    bool operator==(const ShaderLocation &o) const;
    bool operator!=(const ShaderLocation &o) const;

    std::string str() const;

    SR_GETTER(std::string, name);
    SR_GETTER(GLint, location);
    SR_GETTER(GLint, size);
    SR_GETTER(GLenum, type);
  };
}