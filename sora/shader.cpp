﻿/*  Copyright (C) 2011 by if1live */
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

#if SR_ANDROID
#include "gl_inc.h"
#endif

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
}
ShaderProgram::~ShaderProgram() {
  // deinit is manual call
}
void ShaderProgram::Deinit() {
  if (prog != 0) {
    glDeleteProgram(prog);
    prog = 0;
  }
  if (vert_shader_.IsInit()) {
    vert_shader_.Deinit();
  }
  if (frag_shader_.IsInit()) {
    frag_shader_.Deinit();
  }
}
/*
bool ShaderProgram::Link() {
  glLinkProgram(prog);

  GLint status;
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLchar msg[256];
    glGetProgramInfoLog(prog, sizeof(msg), 0, &msg[0]);
    return false;
  }
  return true;
}
*/
bool ShaderProgram::Init(const char *v_src, const char *f_src) {
  SR_ASSERT(prog == 0);

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
void ShaderProgram::Use() {
  glUseProgram(prog);
}
}