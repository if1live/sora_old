﻿/*  Copyright (C) 2011-2012 by if1live */
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

#include "globals.h"

namespace sora {;

struct ShaderVariable {
  typedef ShaderVariableLocation Location;
  typedef ShaderHandle ShaderHandleType;

  ShaderVariable();
  void Set(VarType var_type, VariableHandleType loc_type, const char *attr_name, int size, Location loc, ShaderHandleType shader_handle);

  bool operator==(const ShaderVariable &o) const;
  bool operator!=(const ShaderVariable &o) const;

  std::string str() const;

public:
  VarType var_type; //vec2? mat4? float?
  VariableHandleType location_type;  //attrib/uniform
  std::string name;  //shader코드에서의 변수이름
  int size; //속성의 크기. 쉐이더에서 배열이 아니면 전부1이다
  Location location; //쉐이더에서의 위치값. 연결안되잇으면 일단 -1
  ShaderHandleType shader;  //쉐이더의 핸들
};

struct AttribBindParam {
  int offset;
  int vert_size;
  int var_type;
  int dim;
  bool normalize;
};


struct VertexListBindParam {
  ShaderVariable pos_var;
  ShaderVariable texcoord_var;
  ShaderVariable normal_var;
  ShaderVariable color_var;
  ShaderVariable tangent_var;
};

} //namespace sora

#endif // SORA_SHADER_VARIABLE_H_