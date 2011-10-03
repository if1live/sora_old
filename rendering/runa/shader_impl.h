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
#ifndef RENDERING_RUNA_SHADER_IMPL_H_
#define RENDERING_RUNA_SHADER_IMPL_H_

#include <string>
#include "hayami/hayami.h"

namespace runa {;
template<GLenum ShaderType>
Shader<ShaderType>::Shader()
  : handle_(0) {
}
template<GLenum ShaderType>
Shader<ShaderType>::Shader(const std::string &src)
  : handle_(0) {
    Load(src);
}

template<GLenum ShaderType>
bool Shader<ShaderType>::Load(const std::string &src) {
  handle_ = glCreateShader(ShaderType);
  const char *srcPtr = src.c_str();
  glShaderSource(handle_, 1, &srcPtr, 0);
  glCompileShader(handle_);

  GLint status;
  glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLchar msg[1024];
    glGetShaderInfoLog(handle_, sizeof(msg), 0, &msg[0]);
    const char *srccode = src.c_str();

    hayami::ConsoleLogger logger("Shader");
    logger.Error(srccode);
    SR_ASSERT(!msg);
    return false;
  }
  return true;
}

template<GLenum ShaderType>
Shader<ShaderType>::~Shader() {
  if (handle_ > 0) {
    glDeleteShader(handle_);
    handle_ = 0;
  }
}

template<GLenum ShaderType>
GLuint Shader<ShaderType>::handle() const {
  return handle_;
}
}
#endif  // RENDERING_RUNA_SHADER_IMPL_H_
