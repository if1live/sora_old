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
    kTypeFloat,
    kTypeVec2,
    kTypeVec3,
    kTypeVec4,
    kTypeMat4
  };
  enum {
    kTypeAttrib,
    kTypeUniform,
  };

  static const std::vector<ShaderVariable> &GetPredefinedVarList();
  static const ShaderVariable &GetPredefinedVar(int semantic_code);

public:
  ShaderVariable();
  void Set(int code, int var_type, int loc_type, const char *attr_name);

  int semantic_code;
  int var_type;
  int location_type;
  char name[64];

};

}

#endif  // SORA_SHADER_VARIABLE_H_