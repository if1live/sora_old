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

#if SR_USE_PCH == 0
#include <vector>
#include <string>
#include "gl_inc.h"
#endif

namespace sora {;

struct ShaderVariable {
  enum {
    //uniform type list / variable type list
    kTypeFloat = GL_FLOAT,
    kTypeVec2 = GL_FLOAT_VEC2,
    kTypeVec3 = GL_FLOAT_VEC3,
    kTypeVec4 = GL_FLOAT_VEC4,

    kTypeInt = GL_INT,
    kTypeIntVec2 = GL_INT_VEC2,
    kTypeIntVec3 = GL_INT_VEC3,
    kTypeIntVec4 = GL_INT_VEC4,

    kTypeBool = GL_BOOL,
    kTypeBoolVec2 = GL_BOOL_VEC2,
    kTypeBoolVec3 = GL_BOOL_VEC3,
    kTypeBoolVec4 = GL_BOOL_VEC4,

    kTypeMat2 = GL_FLOAT_MAT2,
    kTypeMat3 = GL_FLOAT_MAT3,
    kTypeMat4 = GL_FLOAT_MAT4,

    kTypeSampler2D = GL_SAMPLER_2D,
    kTypeSamplerCube = GL_SAMPLER_CUBE,
  };
  enum {
    kTypeAttrib,
    kTypeUniform,
  };

public:
  ShaderVariable();
  void Set(int var_type, int loc_type, const char *attr_name, int size);

  bool operator==(const ShaderVariable &o) const;
  bool operator!=(const ShaderVariable &o) const;

  std::string str() const;

public:
  int var_type; //vec2? mat4?
  int location_type;  //attrib/uniform
  std::string name;  //shader코드에서의 변수이름
  int size; //속성의 크기. 쉐이더에서 배열이 아니면 전부1이다
  GLint location; //쉐이더에서의 위치값. 연결안되잇으면 일단 -1
};

}

#endif  // SORA_SHADER_VARIABLE_H_