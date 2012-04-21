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
#include "gl_shader.h"
#include "core/template_lib.h"

#include "sys/memory_file.h"

#if SR_ANDROID
#include "gl_inc.h"
#include <boost/foreach.hpp>
#endif
#include "renderer/globals.h"

using namespace std;
using namespace std::tr1;

namespace sora {;
namespace gl {
  GLHandle::GLHandle()
    : var_type(0),
    location_type(kHandleNone),
    size(0),
    name(""),
    location(-1) {
  }

  void GLHandle::Set(int var_type, HandleType loc_type, const char *attr_name, int size) {
    this->var_type = var_type;
    this->location_type = loc_type;
    this->size = size;
    this->name = attr_name;
    this->location = -1;
  }

  std::string GLHandle::str() const {
    std::ostringstream oss;

    if(location_type == kHandleAttrib) {
      oss << "[Attrib ] " << location << " / " << name;
    } else if(location_type == kHandleUniform) {
      oss << "[Uniform] " << location << " / " << name;
    }

    return oss.str();
  }

  bool GLHandle::operator==(const GLHandle &o) const {
    return (name == o.name 
      && location == o.location
      && size == o.size 
      && var_type == o.var_type
      && location_type == o.location_type
      && location == o.location);
  }
  bool GLHandle::operator!=(const GLHandle &o) const {
    return !(*this == o);
  }

  //////////////////////////////

  GLShader::GLShader()
    : handle(0), type(0) {
  }
  GLShader::~GLShader() {
    // deinit is manual call
  }
  void GLShader::Deinit() {
    if (handle != 0) {
      glDeleteShader(handle);
      handle = 0;
      type = 0;
    }
  }

  bool GLShader::InitShader(GLenum shader_type, const std::vector<const char*> &src_list) {
    SR_ASSERT(handle == 0);
    this->type = shader_type;

    handle = glCreateShader(shader_type);

    //const char *tmp = (src_list[0]);
    //const char **src_list_ptr = &tmp;
    //glGLShaderSource(handle, src_list.size(), src_list_ptr, 0);

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
  bool GLShader::InitVertexShader(const std::vector<const char*> &src_list) {
    return InitShader(GL_VERTEX_SHADER, src_list);
  }
  bool GLShader::InitFragmentShader(const std::vector<const char*> &src_list) {
    return InitShader(GL_FRAGMENT_SHADER, src_list);
  }
  /////////////////////
  GLProgram::GLProgram()
    : prog(0) {
  }
  GLProgram::~GLProgram() {
    // deinit is manual call
  }

  void GLProgram::Deinit() {
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

  bool GLProgram::Init(const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list) {
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
    BOOST_FOREACH(const GLHandle &loc, uniform_var_list_) {
      LOGI("%s", loc.str().c_str());
    }
    attrib_var_list_ = GetActiveAttributeVarList();
    BOOST_FOREACH(const GLHandle &loc, attrib_var_list_) {
      LOGI("%s", loc.str().c_str());
      //모든 attribute를 일단 활성화. 설마disable해놓고 쓰는 일은 없을테니까
      glEnableVertexAttribArray(loc.location);
    }

    return true;
  }

  bool GLProgram::Init(const char *v_src, const char *f_src) {
    vector<const char*> v_src_list;
    vector<const char*> f_src_list;
    v_src_list.push_back(v_src);
    f_src_list.push_back(f_src);
    return Init(v_src_list, f_src_list);
  }

  bool GLProgram::Validate(GLuint prog) {
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
  GLint GLProgram::GetAttribLocation(const char *name) const {
    return glGetAttribLocation(prog, name);
  }

  GLint GLProgram::GetUniformLocation(const char *name) const {
    return glGetUniformLocation(prog, name);
  }


  std::vector<GLHandle> GLProgram::GetActiveUniformVarList() {
    vector<GLHandle> list;

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

      GLHandle sl;
      sl.location = location;
      sl.location_type = kHandleUniform;
      sl.name = uniformName;
      sl.size = size;
      sl.var_type = type;

      list.push_back(sl);
    }

    return list;
  }

  std::vector<GLHandle> GLProgram::GetActiveAttributeVarList() {
    vector<GLHandle> list;

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

      GLHandle sl;
      sl.location = location;
      sl.location_type = kHandleAttrib;
      sl.name = attributeName;
      sl.size = size;
      sl.var_type = type;

      list.push_back(sl);
    }
    return list;
  }

  const GLHandle *GLProgram::uniform_var(const std::string &name) const {
    return FindShaderVar(name, uniform_var_list_);
  }
  const GLHandle *GLProgram::attrib_var(const std::string &name) const {
    return FindShaderVar(name, attrib_var_list_);
  }
  const GLHandle *GLProgram::GetHandle(const std::string &name) const {
    const GLHandle *handle = NULL;
    handle = uniform_var(name);
    if(handle != NULL) {
      return handle;
    }
    handle = attrib_var(name);
    if(handle != NULL) {
      return handle;
    }
    return handle;
  }

  const GLHandle *GLProgram::FindShaderVar(const std::string &name, const std::vector<GLHandle> &var_list) const {
    auto it = var_list.begin();
    auto endit = var_list.end();
    for( ; it != endit ; ++it) {
      if(it->name == name) {
        const GLHandle &var = *it;
        return &var;
      }
    }
    return NULL;
  }
} //namespace gl
} //namespace sora