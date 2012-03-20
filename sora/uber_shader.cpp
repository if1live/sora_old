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
#include "uber_shader.h"
#include "template_lib.h"
#include "filesystem.h"
#include "memory_file.h"

namespace sora {;

using namespace std;

UberShader::UberShader() {
}

bool UberShader::Init() {
  //TODO flag달아서 uber느낌 살리기
  //uber shader 로딩
  std::string app_vert_path = Filesystem::GetAppPath("shader/v_uber.glsl");
  std::string app_frag_path = Filesystem::GetAppPath("shader/f_uber.glsl");
  MemoryFile vert_file(app_vert_path);
  MemoryFile frag_file(app_frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  bool prog_result = prog_.Init(vert_src, frag_src);
  if(prog_result == false) {
    LOGE("Could not create program.");
    //dead???
  }

  //custom var(like light)
  for(int var_code = kAmbientColor ; var_code < kCount ; ++var_code) {
    const ShaderVariable &var_data = GetVariable(var_code);
    int location_type = var_data.location_type;
    const char *location_name = var_data.name;

    if(location_type == ShaderVariable::kTypeAttrib) {
      int location = prog_.GetAttribLocation(location_name);
      prog_.SetLocation(var_code, location);

    } else if(location_type == ShaderVariable::kTypeUniform) {
      int location = prog_.GetUniformLocation(location_name);
      prog_.SetLocation(var_code, location);
    }
  }

  /*
  //show binded variable list
  for(int i = 0 ; i < GetArraySize(location_list_) ; ++i) {
    if(location_list_[i] == -1) {
      continue;
    }

    const ShaderVariable *var = &ShaderVariable::GetPredefinedVar(i);
    if(var->location_type == ShaderVariable::kTypeAttrib) {
      LOGI("Attrib  %s : %d", var->name, location_list_[i]);
    } else if(var->location_type == ShaderVariable::kTypeUniform) {
      LOGI("Uniform %s : %d", var->name, location_list_[i]);
    } else {
      SR_ASSERT(!"not valid");
    }
  }
  */

  return true;
}

UberShader::~UberShader() {
  prog_.Deinit();
}

const std::vector<ShaderVariable> &UberShader::GetVariableList() {
  static bool run = false;
  static vector<ShaderVariable> tuple_list;
  if(run == false) {
    run = true;

    int location_type_table[] = {
      ShaderVariable::kTypeUniform,   //kLocationAmbientColor,
      ShaderVariable::kTypeUniform,   //kLocationDiffuseColor,
      ShaderVariable::kTypeUniform,   //kLocationSpecularColor,
      ShaderVariable::kTypeUniform,   //kLocationSpecularShininess,
      //kLocationTypeUniform,   //kLocationWorldLightPosition
    };
    int var_type_table[] = {
      ShaderVariable::kTypeVec4,   //kLocationAmbientColor,
      ShaderVariable::kTypeVec4,   //kLocationDiffuseColor,
      ShaderVariable::kTypeVec4,   //kLocationSpecularColor,
      ShaderVariable::kTypeFloat,   //kLocationSpecularShininess,
    };

    const char *location_name_table[] = {
      SORA_AMBIENT_COLOR_NAME,           //kLocationAmbientColor,
      SORA_DIFFUSE_COLOR_NAME,           //kLocationDiffuseColor,
      SORA_SPECULAR_COLOR_NAME,          //kLocationSpecularColor,
      SORA_SPECULAR_SHININESS_NAME,      //kLocationSpecularShininess,
    };
    const int type_count = GetArraySize(location_type_table);
    const int name_count = GetArraySize(location_name_table);
    const int var_type_count = GetArraySize(var_type_table);
    SR_ASSERT(type_count == name_count);
    SR_ASSERT(type_count == var_type_count);
    
    for(int code = 0 ; code < type_count ; ++code) {
      int location_type = location_type_table[code];
      const char *location_name = location_name_table[code];
      int var_type = var_type_table[code];

      ShaderVariable v;
      int var_code = ShaderVariable::kSemanticCount + code;
      v.Set(var_code, var_type, location_type, location_name);
      tuple_list.push_back(v);
    }
  }
  return tuple_list;
}
const ShaderVariable &UberShader::GetVariable(int var_code) {
  const vector<ShaderVariable> &var_list = GetVariableList();
  auto it = var_list.begin();
  auto endit = var_list.end();
  for( ; it != endit ; ++it) {
    if(it->semantic_code == var_code) {
      return *it;
    }
  }
  SR_ASSERT(!"not valid");
  static ShaderVariable empty;
  return empty;
}


}
