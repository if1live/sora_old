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
  void GLProgram::Deinit(HandleType *handle) {
    if (*handle != 0) {
      glDeleteProgram(*handle);
      *handle = 0;
    }
  }

  bool GLProgram::Init(HandleType *handle, const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list) {
    if(*handle != 0) {
      Deinit(handle);
    }

    GLShader vert_shader;
    if(false == vert_shader.InitVertexShader(v_src_list)) {
      return false;
    }
    GLShader frag_shader;
    if(false == frag_shader.InitFragmentShader(f_src_list)) {
      return false;
    }

    *handle = glCreateProgram();
    if(*handle == 0) {
      return false;
    }

    glAttachShader(*handle, vert_shader.handle);
    glAttachShader(*handle, frag_shader.handle);

    glLinkProgram(*handle);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(*handle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
      //link fail
      GLint bufLength = 0;
      glGetProgramiv(*handle, GL_INFO_LOG_LENGTH, &bufLength);
      if (bufLength) {
        char* buf = (char*) SR_MALLOC(bufLength);
        if (buf) {
          glGetProgramInfoLog(*handle, bufLength, NULL, buf);
          LOGE("Could not link program:\n%s\n", buf);
          SR_FREE(buf);
        }
      }
      glDeleteProgram(*handle);
      *handle = 0;
      SR_ASSERT(false);
      return false;
    }

    std::vector<ShaderVariable> attrib_var_list = GetActiveAttributeVarList(*handle);
    BOOST_FOREACH(const ShaderVariable &loc, attrib_var_list) {
      //모든 attribute를 일단 활성화. 설마disable해놓고 쓰는 일은 없을테니까
      glEnableVertexAttribArray(loc.location);
    }

    return true;
  }

  bool GLProgram::Init(HandleType *handle, const std::string &v_src, const std::string &f_src) {
    SR_ASSERT(*handle == 0);
    vector<const char*> v_src_list(1, v_src.c_str());
    vector<const char*> f_src_list(1, f_src.c_str());
    return Init(handle, v_src_list, f_src_list);
  }

  bool GLProgram::Validate(HandleType prog) {
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

  std::vector<ShaderVariable> GLProgram::GetActiveUniformVarList(HandleType handle) {
    vector<ShaderVariable> list;

    GLint maxUniformLen;
    GLint numUniform;

    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &numUniform);
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

    const int MaxNameLength = 1024;
    char uniformName[MaxNameLength];
    SR_ASSERT(maxUniformLen < MaxNameLength);
    for (int i = 0 ; i < numUniform ; i++) {
      GLint size;
      GLenum type;
      // get uniform information
      glGetActiveUniform(handle, i, maxUniformLen, NULL, &size, &type, uniformName);
      GLint location = glGetUniformLocation(handle, uniformName);

      ShaderVariable sl;
      sl.location = location;
      sl.location_type = kHandleUniform;
      sl.name = uniformName;
      sl.size = size;
      sl.var_type = GLEnv::GLEnumToVarType(type);
      sl.shader = handle;

      list.push_back(sl);
    }

    return list;
  }

  std::vector<ShaderVariable> GLProgram::GetActiveAttributeVarList(HandleType handle) {
    vector<ShaderVariable> list;

    GLint maxAttributeLen;
    GLint numAttribute;
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &numAttribute);
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);

    const int MaxNameLength = 1024;
    char attributeName[MaxNameLength];
    SR_ASSERT(maxAttributeLen < MaxNameLength);
    for (int i = 0 ; i < numAttribute ; i++) {
      GLint size;
      GLenum type;
      glGetActiveAttrib(handle, i, maxAttributeLen, NULL, &size, &type, attributeName);
      GLint location = glGetAttribLocation(handle, attributeName);

      ShaderVariable sl;
      sl.location = location;
      sl.location_type = kHandleAttrib;
      sl.name = attributeName;
      sl.size = size;
      sl.var_type = GLEnv::GLEnumToVarType(type);
      sl.shader = handle;

      list.push_back(sl);
    }
    return list;
  }

  
  void GLProgram::SetPositionAttrib(HandleType handle, char *base_ptr, const VertexInfo &info) {
    int offset = info.pos_offset;
    if(offset == -1) {
      return;
    }
    //GLShaderVarHandle pos_handle = attrib_var(kPositionHandleName);
    const int vertex_size = info.size;
    int loc = glGetAttribLocation(handle, kPositionHandleName);
    if(loc != -1) {
      GLenum type = info.pos_type;
      //int loc = pos_handle.location;   
      
      int dim = info.pos_dim;
      //glEnableVertexAttribArray(pos_var.location);
      char *ptr = base_ptr + offset;
      glVertexAttribPointer(loc, dim, type, GL_FALSE, vertex_size, ptr);
      SR_CHECK_ERROR("glVertexAttribPointer");
    }
  }
  
  void GLProgram::SetTexcoordAttrib(HandleType handle, char *base_ptr, const VertexInfo &info) {
    int offset = info.texcoord_offset;
    if(offset == -1) {
      return;
    }
    const int vertex_size = info.size;
    //GLShaderVarHandle *texcoord_handle = this->attrib_var(kTexcoordHandleName);
    int loc = glGetAttribLocation(handle, kTexcoordHandleName);
    if(loc != -1) {
      GLenum type = info.texcoord_type;
      //int loc = texcoord_handle->location;
      int loc = glGetAttribLocation(handle, kTexcoordHandleName);
      int dim = info.texcoord_dim;
      //glEnableVertexAttribArray(texcoord_var.location);
      char *ptr = base_ptr + offset;
      glVertexAttribPointer(loc, dim, type, GL_FALSE, vertex_size, ptr);
      SR_CHECK_ERROR("glVertexAttribPointer");
    }
  }
  
  void GLProgram::SetNormalAttrib(HandleType handle, char *base_ptr, const VertexInfo &info) {
    int offset = info.normal_offset;
    if(offset == -1) {
      return;
    }
    const int vertex_size = info.size;
    //const GLShaderVarHandle *normal_handle = this->attrib_var(kNormalHandleName);
    int loc = glGetAttribLocation(handle, kNormalHandleName);
    if(loc != -1) {
      GLenum type = info.normal_type;
      //int loc = normal_handle->location;
      
      int dim = info.normal_dim;
      char *ptr = base_ptr + offset;
      glEnableVertexAttribArray(loc);
      glVertexAttribPointer(loc, dim, type, GL_FALSE, vertex_size, ptr);
      SR_CHECK_ERROR("glVertexAttribPointer");
    }
  }
  
  void GLProgram::SetColorAttrib(HandleType handle, char *base_ptr, const VertexInfo &info) {
    int offset = info.color_offset;
    if(offset == -1) {
      return;
    }
    const int vertex_size = info.size;
    //const GLShaderVarHandle *color_handle = this->attrib_var(kColorHandleName);
    int loc = glGetAttribLocation(handle, kColorHandleName);
    if(loc != -1) {
      GLenum type = info.color_type;
      //int loc = color_handle->location;
      
      //색속성은 ubyte니까 normalize해야됨
      char *ptr = base_ptr + offset;
      int dim = info.color_dim;
      glEnableVertexAttribArray(loc);
      glVertexAttribPointer(loc, dim, type, GL_TRUE, vertex_size, ptr);
      SR_CHECK_ERROR("glVertexAttribPointer");
    }
  }
  
  void GLProgram::SetTangentAttrib(HandleType handle, char *base_ptr, const VertexInfo &info) {
    int offset = info.tangent_offset;
    if(offset == -1) {
      return;
    }
    const int vertex_size = info.size;
    //const GLShaderVarHandle *tangent_handle = this->attrib_var(kTangentHandleName);
    int loc = glGetAttribLocation(handle, kTangentHandleName);
    if(loc != -1) {
      GLenum type = info.tangent_type;
      //int loc = tangent_handle->location;
      
      char *ptr = base_ptr + offset;
      int dim = info.tangent_dim;
      glEnableVertexAttribArray(loc);
      glVertexAttribPointer(loc, dim, type, GL_FALSE, vertex_size, ptr);
      SR_CHECK_ERROR("glVertexAttribPointer");
    }
  }
  
} //namespace gl
} //namespace sora