/*  Copyright (C) 2011-2012 by if1live */
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
#ifndef SORA_UBER_SHADER_H_
#define SORA_UBER_SHADER_H_

#include "shader.h"
#include "shader_variable.h"

#define SORA_AMBIENT_COLOR_NAME         "u_ambientColor"
#define SORA_DIFFUSE_COLOR_NAME         "u_diffuseColor"
#define SORA_SPECULAR_COLOR_NAME        "u_specularColor"
#define SORA_SPECULAR_SHININESS_NAME    "u_specularShininess"
//#define SORA_WORLD_LIGHT_POSITION_NAME  "u_worldLightPosition"

namespace sora {;
//ambient / diffuse / specular같은거 기본 지원하기 위한 용도
//설계 테스트용
class SR_DLL UberShader {
public:
  enum {
    kAmbientColor = ShaderVariable::kSemanticCount,
    kDiffuseColor,
    kSpecularColor,
    kSpecularShininess,
    //kLocationWorldLightPosition,
    kCount,
  };
  
public:
  UberShader();
  ~UberShader();

  bool Init();

  static const std::vector<ShaderVariable> &GetVariableList();
  static const ShaderVariable &GetVariable(int var_code);

  const ShaderProgram &prog() { return prog_; }

private:
  ShaderProgram prog_;
};
}

#endif  // SORA_UBER_SHADER_H_