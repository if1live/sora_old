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
#ifndef SORA_SHADER_H_
#define SORA_SHADER_H_

#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#if SR_USE_PCH == 0
#include <string>
#include "core/tuple_inc.h"
#include <vector>
#endif

#include "shader_variable.h"
#include "shader_bind_policy.h"

namespace sora {;

class SR_DLL Shader {
public:
  Shader();
  ~Shader();

  bool InitVertexShader(const std::vector<const char*> &src_list);
  bool InitFragmentShader(const std::vector<const char*> &src_list);
  bool InitVertexShader(const char *src);
  bool InitFragmentShader(const char *src);
  bool InitVertexShader(const std::string &src) {
    return InitVertexShader(src.c_str());
  }
  bool InitFragmentShader(const std::string &src) {
    return InitFragmentShader(src.c_str());
  }
  void Deinit();
  bool IsInit() const { return (handle != 0); }
  
private:
  bool InitShader(GLenum shader_type, const char *src);
  bool InitShader(GLenum shader_type, const std::vector<const char*> &src_list);
public:
  GLuint handle;
  GLenum type;
};

class SR_DLL ShaderProgram {
public:
  static bool Validate(GLuint prog);

public:
  ShaderProgram();
  ~ShaderProgram();

  bool Init(const char *v_src, const char *f_src);
  bool Init(const std::string &v_src, const std::string &f_src) {
    return Init(v_src.c_str(), f_src.c_str());
  }
  bool Init(const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list);

  void Deinit();
  bool IsInit() const { return (prog != 0); }

  GLint GetAttribLocation(const char *name) const;
  GLint GetUniformLocation(const char *name) const;

  //최초실행시에는 모든목록을 얻을수잇다
  std::vector<ShaderVariable> GetActiveUniformVarList();
  std::vector<ShaderVariable> GetActiveAttributeVarList();

  const ShaderVariable *uniform_var(const char *name) const {
    return uniform_var(std::string(name));
  }
  const ShaderVariable *attrib_var(const char *name) const {
    return attrib_var(std::string(name));
  }
  const ShaderVariable *uniform_var(const std::string &name) const;
  const ShaderVariable *attrib_var(const std::string &name) const;

public:
  GLuint prog;
  ShaderBindPolicy bind_policy;

private:
  Shader vert_shader_;
  Shader frag_shader_;

  std::vector<ShaderVariable> uniform_var_list_;
  std::vector<ShaderVariable> attrib_var_list_;
  const ShaderVariable *FindShaderVar(const std::string &name, const std::vector<ShaderVariable> &var_list) const;
};
}
#endif  // SORA_SHADER_H_