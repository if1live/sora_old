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

#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;

const char kViewDirectionName[] = "u_viewDirection";
const char kViewPositionName[] = "u_viewPosition";
const char kViewSideName[] = "u_viewSide";
const char kViewUpName[] = "u_viewUp";

const char kViewName[] = "u_view";
const char kProjectionName[] = "u_projection";
const char kWorldViewProjectionName[] = "u_worldViewProjection";
const char kWorldName[] = "u_world";

const char kPositionName[] = "a_position";
const char kNormalName[] = "a_normal";
const char kTexcoordName[] = "a_texcoord";

//render monkey predefine list
struct ShaderVariable {
  enum {
    kUnknown = -1,
    //predefined semantic list
    //vec4
    kViewDirection,
    kViewPosition,
    kViewSide,
    kViewUp,

    //mat4
    //kViewProjection,
    //kViewProjectionInverse,
    //kViewProjectionTranspose,
    //kViewProjectionInverseTranspose,

    kView,
    //kViewInverse,
    //kViewTranspose,
    //kViewInverseTrsnspose,

    kProjection,
    //kProjectionInverse,
    //kProjectionTranspose,
    //kProjectionInverseTranspose,

    kWorldViewProjection,
    //kWorldViewProjectionInverse,
    //kWorldViewProjectionTranspose,
    //kWorldViewProjectionInverseTranspose,

    kWorld,
    //kWorldInverse,
    //kWorldTranspose,
    //kWorldInverseTranspose,

    //kWorldView,
    //kWorldViewInverse,
    //kWorldViewTranspose,    
    //kWorldViewInverseTranspose,

    //position, normal...등등 항상 뻔한것들까지 일단 떄려박자
    kPosition,
    kNormal,
    kTexcoord,

    kSemanticCount,
  };
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

  static const std::vector<ShaderVariable> &GetPredefinedVarList();
  static const ShaderVariable &GetPredefinedVar(int semantic_code);

public:
  ShaderVariable();
  void Set(int code, int var_type, int loc_type, const char *attr_name, int size);

  int semantic_code;  //렌더러 구현에서 쓰이는 의미를 가지는 코드 kModel이라든가 그런것들
  int var_type; //vec2? mat4?
  int location_type;  //attrib/uniform
  char name[64];  //shader코드에서의 변수이름
  int size; //속성의 크기
};

////////////////////////////////////////

//쉐이더 위치 정보. 쌩으로 덤프뜰때 사용한다
class ShaderLocation {
public:
  ShaderLocation(int loc_type, const char *name, GLint location, GLint size, GLenum type);

  bool operator==(const ShaderLocation &o) const;
  bool operator!=(const ShaderLocation &o) const;

  std::string str() const;

  int location_type;  //attrib/uniform
  std::string name;
  GLint location;
  GLint size;
  GLenum type;
};


}

#endif  // SORA_SHADER_VARIABLE_H_