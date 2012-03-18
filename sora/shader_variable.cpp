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

using namespace std;

namespace sora {;
ShaderVariable::ShaderVariable()
: semantic_code(kUnknown), var_type(0), location_type(0) {
  strcpy(name, ""); 
}

void ShaderVariable::Set(int code, int var_type, int loc_type, const char *attr_name) {
  this->semantic_code = code;
  this->var_type = var_type;
  this->location_type = loc_type;
  strcpy(name, attr_name);
}

const std::vector<ShaderVariable> &ShaderVariable::GetPredefinedVarList() {
  static vector<ShaderVariable> var_list(kSemanticCount);
  static bool run = false;
  if(run == false) {
    run = true;
    ShaderVariable v;

    v.Set(kViewDirection, kTypeVec4, kTypeUniform, kViewDirectionName);
    var_list[kViewDirection] = v;
    
    v.Set(kViewPosition, kTypeVec4, kTypeUniform, kViewPositionName);
    var_list[kViewPosition] = v;

    v.Set(kViewSide, kTypeVec4, kTypeUniform, kViewSideName);
    var_list[kViewSide] = v;

    v.Set(kViewUp, kTypeVec4, kTypeUniform, kViewUpName);
    var_list[kViewUp] = (v);

    v.Set(kView, kTypeMat4, kTypeUniform, kViewName);
    var_list[kView] = (v);

    v.Set(kProjection, kTypeMat4, kTypeUniform, kProjectionName);
    var_list[kProjection] = (v);

    v.Set(kWorldViewProjection, kTypeMat4, kTypeUniform, kWorldViewProjectionName);
    var_list[kWorldViewProjection] = (v);

    v.Set(kWorld, kTypeMat4, kTypeUniform, kWorldName);
    var_list[kWorld] = (v);

    v.Set(kPosition, kTypeVec3, kTypeAttrib, kPositionName);
    var_list[kPosition] = v;

    v.Set(kNormal, kTypeVec3, kTypeAttrib, kNormalName);
    var_list[kNormal] = (v);

    v.Set(kTexcoord, kTypeVec2, kTypeAttrib, kTexcoordName);
    var_list[kTexcoord] = (v);
  }
  return var_list;
}
const ShaderVariable &ShaderVariable::GetPredefinedVar(int semantic_code) {
  if(semantic_code >= 0 && semantic_code < kSemanticCount) {
    const ShaderVariable &var = GetPredefinedVarList().at(semantic_code);
    SR_ASSERT(var.semantic_code == semantic_code);
    return var;
  }

  //else...
  SR_ASSERT(!"not valid");
  static ShaderVariable empty;
  return empty;
}
}