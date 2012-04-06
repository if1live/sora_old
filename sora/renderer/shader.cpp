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
#include "core/template_lib.h"
#include "shader_variable.h"

#include "sys/memory_file.h"

#if SR_ANDROID
#include "gl_inc.h"
#include <boost/foreach.hpp>
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

bool Shader::InitShader(GLenum shader_type, const std::vector<const char*> &src_list) {
  SR_ASSERT(handle == 0);
  this->type = shader_type;

  handle = glCreateShader(shader_type);

  //const char *tmp = (src_list[0]);
  //const char **src_list_ptr = &tmp;
  //glShaderSource(handle, src_list.size(), src_list_ptr, 0);

  string src;
  for(size_t i = 0 ; i < src_list.size() ; i++) {
    src += src_list[i];
  }
  const char *tmp = src.c_str();
  const char **src_ptr = &tmp;
  glShaderSource(handle, 1, src_ptr, 0);


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

      string src;
      for(size_t i = 0 ; i < src_list.size() ; i++) {
        src += src_list[i];
        src += '\n';
      }
      LOGE("ShaderSrc : %s", src.c_str());
      SR_ASSERT(false);
    }
    return false;
  }
  return true;
}
bool Shader::InitVertexShader(const std::vector<const char*> &src_list) {
  return InitShader(GL_VERTEX_SHADER, src_list);
}
bool Shader::InitFragmentShader(const std::vector<const char*> &src_list) {
  return InitShader(GL_FRAGMENT_SHADER, src_list);
}
/////////////////////
ShaderProgram::ShaderProgram()
: prog(0) {
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
    bind_policy = ShaderBindPolicy();
  }
}

bool ShaderProgram::Init(const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list) {
  if(prog != 0) {
    Deinit();
  }

  if(false == vert_shader_.InitVertexShader(v_src_list)) {
    return false;
  }
  if(false == frag_shader_.InitFragmentShader(f_src_list)) {
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

  LOGI("Active Uniform/Attrib list");
  uniform_var_list_ = GetActiveUniformVarList();
  BOOST_FOREACH(const ShaderVariable &loc, uniform_var_list_) {
    LOGI("%s", loc.str().c_str());
  }
  attrib_var_list_ = GetActiveAttributeVarList();
  BOOST_FOREACH(const ShaderVariable &loc, attrib_var_list_) {
    LOGI("%s", loc.str().c_str());
  }

  bind_policy = ShaderBindPolicy(prog);
  return true;
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

  LOGI("Active Uniform/Attrib list");
  uniform_var_list_ = GetActiveUniformVarList();
  BOOST_FOREACH(const ShaderVariable &loc, uniform_var_list_) {
    LOGI("%s", loc.str().c_str());
  }
  attrib_var_list_ = GetActiveAttributeVarList();
  BOOST_FOREACH(const ShaderVariable &loc, attrib_var_list_) {
    LOGI("%s", loc.str().c_str());
  }

  bind_policy = ShaderBindPolicy(prog);
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

std::vector<ShaderVariable> ShaderProgram::GetActiveUniformVarList() {
  vector<ShaderVariable> list;
  
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

    ShaderVariable sl;
    sl.location = location;
    sl.location_type = ShaderVariable::kTypeUniform;
    sl.name = uniformName;
    sl.size = size;
    sl.var_type = type;

    list.push_back(sl);
  }

  return list;
}

std::vector<ShaderVariable> ShaderProgram::GetActiveAttributeVarList() {
  vector<ShaderVariable> list;

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

    ShaderVariable sl;
    sl.location = location;
    sl.location_type = ShaderVariable::kTypeAttrib;
    sl.name = attributeName;
    sl.size = size;
    sl.var_type = type;

    list.push_back(sl);
  }
  return list;
}

const ShaderVariable *ShaderProgram::uniform_var(const std::string &name) const {
  return FindShaderVar(name, uniform_var_list_);
}
const ShaderVariable *ShaderProgram::attrib_var(const std::string &name) const {
  return FindShaderVar(name, attrib_var_list_);
}

const ShaderVariable *ShaderProgram::FindShaderVar(const std::string &name, const std::vector<ShaderVariable> &var_list) const {
  auto it = var_list.begin();
  auto endit = var_list.end();
  for( ; it != endit ; ++it) {
    if(it->name == name) {
      const ShaderVariable &var = *it;
      return &var;
    }
  }
  return NULL;
}

void ShaderProgram::BuildBindPolicy() {
  bind_policy.Clear();

  //bind되는 변수는 uber shader의 경우는 코드레벨에서 떄려박을수 있다
  vector<ShaderNameBind> &attr_bind_param = ShaderBindPolicy::GetPredefinedAttribList();
  for(size_t i = 0 ; i < attr_bind_param.size() ; i++) {
    const ShaderNameBind &param = attr_bind_param[i];
    const ShaderVariable *var = attrib_var(param.name);
    if(var != NULL) {
      bind_policy.set_var(param.semantic, *var);
    }
  }

  vector<ShaderNameBind> &uniform_bind_param = ShaderBindPolicy::GetPredefinedUniformList();
  for(size_t i = 0 ; i < uniform_bind_param.size() ; i++) {
    const ShaderNameBind &param = uniform_bind_param[i];
    const ShaderVariable *var = uniform_var(param.name);
    if(var != NULL) {
      bind_policy.set_var(param.semantic, *var);
    }
  }
}

bool ShaderProgram::LoadFromFile(const std::string &vert_path, const std::string &frag_path) {
  sora::MemoryFile vert_file(vert_path);
  sora::MemoryFile frag_file(frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  bool prog_result = Init(vert_src, frag_src);
  if(prog_result == false) {
    LOGE("Could not create program.");
  }
  BuildBindPolicy();  //사실상 모든 쉐이더에서 쓰니까 그냥 내장해놓자
  return prog_result;
}
} //namespace sora