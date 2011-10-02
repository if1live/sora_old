/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "precompile.h"
#include "runa/shader_location.h"

namespace runa {;
  ShaderLocation::ShaderLocation(const char *name,
    GLint location, GLint size, GLenum type)
    : name_(name),
    location_(location),
    size_(size),
    type_(type) {
  }
  std::string ShaderLocation::str() const {
    std::ostringstream oss;
    oss << "[Uniform]" << location() << "/" << name();
    return oss.str();
  }

  bool ShaderLocation::operator==(const ShaderLocation &o) const {
    return (name_ == o.name_ && location_ == o.location_
      && size_ == o.size_ && type_ == o.type_);
  }
  bool ShaderLocation::operator!=(const ShaderLocation &o) const {
    return !(*this == o);
  }
}
