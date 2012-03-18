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
#include "tuple_inc.h"
#include <vector>
#endif

#include "shader_variable.h"

#define SORA_AMBIENT_COLOR_NAME         "u_ambientColor"
#define SORA_DIFFUSE_COLOR_NAME         "u_diffuseColor"
#define SORA_SPECULAR_COLOR_NAME        "u_specularColor"
#define SORA_SPECULAR_SHININESS_NAME    "u_specularShininess"
//#define SORA_WORLD_LIGHT_POSITION_NAME  "u_worldLightPosition"

namespace sora {;

enum {
  kLocationAmbientColor = ShaderVariable::kSemanticCount,
  kLocationDiffuseColor,
  kLocationSpecularColor,
  kLocationSpecularShininess,
  //kLocationWorldLightPosition,
  kLocationCount,
};

class SR_DLL Shader {
public:
  Shader();
  ~Shader();

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

public:
  GLuint handle;
  GLenum type;
};

class SR_DLL ShaderProgram {
public:
  static bool Validate(GLuint prog);

  static const std::vector<ShaderVariable> &GetVariableList();
  static const ShaderVariable &GetVariable(int var_code);
public:
  ShaderProgram();
  ~ShaderProgram();

  bool Init(const char *v_src, const char *f_src);
  bool Init(const std::string &v_src, const std::string &f_src) {
    return Init(v_src.c_str(), f_src.c_str());
  }
  void Deinit();
  bool IsInit() const { return (prog != 0); }
  //void Use();

  GLint GetAttribLocation(const char *name);
  GLint GetUniformLocation(const char *name);
  int GetLocation(int location_code) { return location_list_[location_code]; }

public:
  GLuint prog;

private:
  Shader vert_shader_;
  Shader frag_shader_;


  //predefined + shader custom part
  int location_list_[kLocationCount];
};

}
#endif  // SORA_SHADER_H_