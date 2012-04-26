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
#ifndef SORA_SHADER_VARIABLE_H_
#define SORA_SHADER_VARIABLE_H_

#include "renderer/gl/gl_env.h"

namespace sora {;
struct ShaderVariable {
  typedef sora::gl::ShaderVariableHandleType HandleType;

  ShaderVariable();
  void Set(VarType var_type, HandleType loc_type, const char *attr_name, int size, HandleType loc);

  bool operator==(const ShaderVariable &o) const;
  bool operator!=(const ShaderVariable &o) const;

  std::string str() const;

public:
  VarType var_type; //vec2? mat4? float?
  HandleType location_type;  //attrib/uniform
  std::string name;  //shader코드에서의 변수이름
  int size; //속성의 크기. 쉐이더에서 배열이 아니면 전부1이다
  HandleType location; //쉐이더에서의 위치값. 연결안되잇으면 일단 -1
};
}

#endif // SORA_SHADER_VARIABLE_H_