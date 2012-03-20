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
      char* buf = (char*) SR_MALLOC(infoLen);
      if (buf) {
        glGetShaderInfoLog(handle, infoLen, NULL, buf);
        LOGE("Could not compile shader %d:\n%s\n", shader_type, buf);
        SR_FREE(buf);
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
    for(int i = 0 ; i < GetArraySize(location_list_) ; i++) {
      location_list_[i] = -1;
    }
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
      char* buf = (char*) SR_MALLOC(bufLength);
      if (buf) {
        glGetProgramInfoLog(prog, bufLength, NULL, buf);
        LOGE("Could not link program:\n%s\n", buf);
        SR_FREE(buf);
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

  //show binded variable list
  LOGI("Predefined Uniform/Attrib list");
  for(int i = 0 ; i < ShaderVariable::kSemanticCount ; ++i) {
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

  LOGI("Active Uniform/Attrib list");
  vector<ShaderLocation> uniform_list = GetActiveUniformLocationList();
  BOOST_FOREACH(const ShaderLocation &loc, uniform_list) {
    LOGI("%s", loc.str().c_str());
  }
  vector<ShaderLocation> attrib_list = GetActiveAttributeLocationList();
  BOOST_FOREACH(const ShaderLocation &loc, attrib_list) {
    LOGI("%s", loc.str().c_str());
  }

  return true;
}

bool ShaderProgram::Validate(GLuint prog) {
  GLint logLength, status;

  glValidateProgram(prog);
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    GLchar *log = static_cast<GLchar *>(SR_MALLOC(logLength));
    glGetProgramInfoLog(prog, logLength, &logLength, log);
    SR_FREE(log);
    //SR_ASSERT(false);
  }

  glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
  if (status == 0) {
    SR_ASSERT(!"validate fail");
    return false;
  }
  return true;
}
GLint ShaderProgram::GetAttribLocation(const char *name) const {
  return glGetAttribLocation(prog, name);
}

GLint ShaderProgram::GetUniformLocation(const char *name) const {
  return glGetUniformLocation(prog, name);
}
//void ShaderProgram::Use() {
//  glUseProgram(prog);
//}

int ShaderProgram::GetLocation(int location_code) const {
  SR_ASSERT(location_code >= 0);
  SR_ASSERT(location_code < kMaxLocationCount);
  return location_list_[location_code]; 
}
void ShaderProgram::SetLocation(int location_code, int loc) {
  SR_ASSERT(location_code >= 0);
  SR_ASSERT(location_code < kMaxLocationCount);
  location_list_[location_code] = loc; 
}

std::vector<ShaderLocation> ShaderProgram::GetActiveUniformLocationList() {
  vector<ShaderLocation> list;
  
  GLint maxUniformLen;
  GLint numUniform;

  glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &numUniform);
  glGetProgramiv(prog, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

  const int MaxNameLength = 1024;
  char uniformName[MaxNameLength];
  SR_ASSERT(maxUniformLen < MaxNameLength);
  for (int i = 0 ; i < numUniform ; i++) {
    GLint size;
    GLenum type;
    // get uniform information
    glGetActiveUniform(prog, i, maxUniformLen, NULL, &size, &type, uniformName);
    GLint location = glGetUniformLocation(prog, uniformName);

    ShaderLocation sl(ShaderVariable::kTypeUniform, uniformName, location, size, type);
    list.push_back(sl);
  }

  return list;
}

std::vector<ShaderLocation> ShaderProgram::GetActiveAttributeLocationList() {
  vector<ShaderLocation> list;

  GLint maxAttributeLen;
  GLint numAttribute;
  glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &numAttribute);
  glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);

  const int MaxNameLength = 1024;
  char attributeName[MaxNameLength];
  SR_ASSERT(maxAttributeLen < MaxNameLength);
  for (int i = 0 ; i < numAttribute ; i++) {
    GLint size;
    GLenum type;
    glGetActiveAttrib(prog, i, maxAttributeLen, NULL, &size, &type, attributeName);
    GLint location = glGetAttribLocation(prog, attributeName);

    ShaderLocation sl(ShaderVariable::kTypeAttrib, attributeName, location, size, type);
    list.push_back(sl);
  }
  return list;
}

} //namespace sora