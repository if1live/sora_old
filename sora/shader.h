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
#include <string>

#define SORA_MODELVIEWPROJECTION_NAME   "u_modelViewProjection"
#define SORA_POSITION_NAME              "a_position"
#define SORA_NORMAL_NAME                "a_normal"
#define SORA_TEXCOORD_NAME              "a_texcoord"
#define SORA_AMBIENT_COLOR_NAME         "u_ambientColor"
#define SORA_DIFFUSE_COLOR_NAME         "u_diffuseColor"
#define SORA_SPECULAR_COLOR_NAME        "u_specularColor"
#define SORA_SPECULAR_SHININESS_NAME    "u_specularShininess"
#define SORA_MATERIAL_ALPHA_NAME        "u_materialAlpha"
#define SORA_LIGHT_POSITION_NAME        "u_lightPosition"

namespace sora {;

enum {
  kLocationModelViewProjection,
  kLocationPosition,
  kLocationNormal,
  kLocationTexcoord,
  kLocationAmbientColor,
  kLocationDiffuseColor,
  kLocationSpecularColor,
  kLocationSpecularShininess,
  kLocationMaterialAlpha,
  kLocationLightPosition,
  kLocationCount,
};
enum {
  kLocationTypeAttrib,
  kLocationTypeUniform,
  kLocationTypeCount,
};
//0 : location code
//1 : attrib, uniform
//2 : name
typedef std::tr1::tuple<int, int, const char*> LocationTuple;

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

  static const std::vector<LocationTuple> &GetLocationTupleList();
  static const LocationTuple &GetLocationTuple(int location);
public:
  ShaderProgram();
  ~ShaderProgram();

  bool Init(const char *v_src, const char *f_src);
  bool Init(const std::string &v_src, const std::string &f_src) {
    return Init(v_src.c_str(), f_src.c_str());
  }
  void Deinit();
  bool IsInit() const { return (prog != 0); }
  void Use();

  GLint GetAttribLocation(const char *name);
  GLint GetUniformLocation(const char *name);
  int GetLocation(int location_code) { return location_list_[location_code]; }

public:
  GLuint prog;

private:
  Shader vert_shader_;
  Shader frag_shader_;

  int location_list_[kLocationCount];
};

}
#endif  // SORA_SHADER_H_