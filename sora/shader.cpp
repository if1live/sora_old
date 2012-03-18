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
#include "sora_stdafx.h"
#include "shader.h"
#include "template_lib.h"
#include "shader_variable.h"

#if SR_ANDROID
#include "gl_inc.h"
#endif

using namespace std;
using namespace std::tr1;

namespace sora {;
Shader::Shader()
  : handle(0), type(0) {
}
Shader::~Shader() {
  // deinit is manual call
}
void Shader::Deinit() {
  if (handle != 0) {
    glDeleteShader(handle);
    handle = 0;
    type = 0;
  }
}

bool Shader::InitVertexShader(const char *src) {
  return InitShader(GL_VERTEX_SHADER, src);
}
bool Shader::InitFragmentShader(const char *src) {
  return InitShader(GL_FRAGMENT_SHADER, src);
}
bool Shader::InitShader(GLenum shader_type, const char *src) {
  SR_ASSERT(handle == 0);
  this->type = shader_type;

  handle = glCreateShader(shader_type);
  glShaderSource(handle, 1, &src, 0);
  glCompileShader(handle);

  GLint status;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLen = 0;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen) {
      char* buf = (char*) malloc(infoLen);
      if (buf) {
        glGetShaderInfoLog(handle, infoLen, NULL, buf);
        LOGE("Could not compile shader %d:\n%s\n", shader_type, buf);
        free(buf);
      }
      glDeleteShader(handle);
      handle = 0;

      LOGE("ShaderSrc : %s", src);
      SR_ASSERT(false);
    }
    return false;
  }
  return true;
}
/////////////////////
ShaderProgram::ShaderProgram()
: prog(0) {
  memset(location_list_, 0, sizeof(location_list_));
}
ShaderProgram::~ShaderProgram() {
  // deinit is manual call
}

void ShaderProgram::Deinit() {
  if (vert_shader_.IsInit()) {
    vert_shader_.Deinit();
  }
  if (frag_shader_.IsInit()) {
    frag_shader_.Deinit();
  }
  if (prog != 0) {
    glDeleteProgram(prog);
    prog = 0;
  }
}

bool ShaderProgram::Init(const char *v_src, const char *f_src) {
  if(prog != 0) {
    Deinit();
  }

  if(false == vert_shader_.InitVertexShader(v_src)) {
    return false;
  }
  if(false == frag_shader_.InitFragmentShader(f_src)) {
    return false;
  }

  prog = glCreateProgram();
  if(!prog) {
    return false;
  }

  glAttachShader(prog, vert_shader_.handle);
  glAttachShader(prog, frag_shader_.handle);

  glLinkProgram(prog);
  GLint linkStatus = GL_FALSE;
  glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
  if (linkStatus != GL_TRUE) {
    //link fail
    GLint bufLength = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &bufLength);
    if (bufLength) {
      char* buf = (char*) malloc(bufLength);
      if (buf) {
        glGetProgramInfoLog(prog, bufLength, NULL, buf);
        LOGE("Could not link program:\n%s\n", buf);
        free(buf);
      }
    }
    glDeleteProgram(prog);
    prog = 0;
    SR_ASSERT(false);
    return false;
  }

  //semantic var 
  for(int var_code = 0 ; var_code < ShaderVariable::kSemanticCount ; ++var_code) {
    const ShaderVariable &var_data = ShaderVariable::GetPredefinedVar(var_code);
    int location_type = var_data.location_type;
    const char *location_name = var_data.name;

    if(location_type == ShaderVariable::kTypeAttrib) {
      int location = GetAttribLocation(location_name);
      location_list_[var_code] = location;

    } else if(location_type == ShaderVariable::kTypeUniform) {
      int location = GetUniformLocation(location_name);
      location_list_[var_code] = location;
    }
  }

  //custom var(like light)
  for(int var_code = ShaderVariable::kSemanticCount ; var_code < kLocationCount ; ++var_code) {
    const ShaderVariable &var_data = GetVariable(var_code);
    int location_type = var_data.location_type;
    const char *location_name = var_data.name;

    if(location_type == ShaderVariable::kTypeAttrib) {
      int location = GetAttribLocation(location_name);
      location_list_[var_code] = location;

    } else if(location_type == ShaderVariable::kTypeUniform) {
      int location = GetUniformLocation(location_name);
      location_list_[var_code] = location;
    }
  }

  //show binded variable list
  for(int i = 0 ; i < GetArraySize(location_list_) ; ++i) {
    if(location_list_[i] == -1) {
      continue;
    }

    const ShaderVariable *var = NULL;
    if(i < ShaderVariable::kSemanticCount) {
      const ShaderVariable &v = ShaderVariable::GetPredefinedVar(i);
      var = &v;
    } else {
      const ShaderVariable &v = GetVariable(i);
      var = &v;
    }

    if(var->location_type == ShaderVariable::kTypeAttrib) {
      LOGI("Attrib  %s : %d", var->name, location_list_[i]);
    } else if(var->location_type == ShaderVariable::kTypeUniform) {
      LOGI("Uniform %s : %d", var->name, location_list_[i]);
    } else {
      SR_ASSERT(!"not valid");
    }
  }

  return true;
}

bool ShaderProgram::Validate(GLuint prog) {
    GLint logLength, status;

  glValidateProgram(prog);
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    GLchar *log = static_cast<GLchar *>(malloc(logLength));
    glGetProgramInfoLog(prog, logLength, &logLength, log);
    free(log);
    //SR_ASSERT(false);
  }

  glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
  if (status == 0) {
    SR_ASSERT(!"validate fail");
    return false;
  }
  return true;
}
GLint ShaderProgram::GetAttribLocation(const char *name) {
  return glGetAttribLocation(prog, name);
}

GLint ShaderProgram::GetUniformLocation(const char *name) {
  return glGetUniformLocation(prog, name);
}
//void ShaderProgram::Use() {
//  glUseProgram(prog);
//}

const std::vector<ShaderVariable> &ShaderProgram::GetVariableList() {
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
const ShaderVariable &ShaderProgram::GetVariable(int var_code) {
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