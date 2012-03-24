/*  Copyright (C) 2011-2012 by if1live */
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
#include "sora_stdafx.h"
#include "shader_variable.h"

#if SR_USE_PCH == 0
#include <sstream>
#endif

using namespace std;

namespace sora {;
ShaderVariable::ShaderVariable()
  : var_type(0),
  location_type(0),
  size(0),
  name(""),
  location(-1) {
}

void ShaderVariable::Set(int var_type, int loc_type, const char *attr_name, int size) {
  this->var_type = var_type;
  this->location_type = loc_type;
  this->size = size;
  this->name = attr_name;
  this->location = -1;
}

std::string ShaderVariable::str() const {
  std::ostringstream oss;

  if(location_type == ShaderVariable::kTypeAttrib) {
    oss << "[Attrib ] " << location << " / " << name;
  } else if(location_type == ShaderVariable::kTypeUniform) {
    oss << "[Uniform] " << location << " / " << name;
  }
  
  return oss.str();
}

bool ShaderVariable::operator==(const ShaderVariable &o) const {
  return (name == o.name 
    && location == o.location
    && size == o.size 
    && var_type == o.var_type
    && location_type == o.location_type
    && location == o.location);
}
bool ShaderVariable::operator!=(const ShaderVariable &o) const {
  return !(*this == o);
}
}