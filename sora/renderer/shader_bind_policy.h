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
#ifndef SORA_SHADER_BIND_POLICY_H_
#define SORA_SHADER_BIND_POLICY_H_

#include "shader_variable.h"

namespace sora {;

//쉐이더에서의 변수 이름과 해당거싱 가질 의미를 미리 설정해놓을수 잇도록하자
//쉐이더 코드가 여러개가 되더라고 항상쓰는 변수명은 뻔할테니까 이런짓을 해도 큰 문제는 없을 것이다
struct ShaderNameBind{
  ShaderNameBind(const char *name, int semantic)
    : name(name), semantic(semantic) {}
  std::string name;
  int semantic;
};

class ShaderBindPolicy {
public:
  enum {
    kUnknown = -1,
    
    //camera
    kViewDirection,
    kViewPosition,
    kViewSide,
    kViewUp,

    //basic matrix
    kView,
    kProjection,
    kWorld,
    kWorldViewProjection,
    
    //model data
    kPosition,
    kNormal,
    kColor,
    kTexcoord,
    kTangent,
    kBinormal,

    //디버깅에 쓰일수 잇는 특수 속성
    kConstColor,

    //빛+재질은 섞어서 한번에 다룰수 있도록 하자
    kAmbientColor,
    kDiffuseColor,
    kSpecularColor,
    kSpecularShininess,
    kLightPosition,
    kAmbientMap,
    kDiffuseMap,
    kSpecularMap,

    /*
    kLightAmbient,
    kLightDiffuse,
    kLightSpecular,
    kLightShininess,
    kLightPosition,

    //재질
    kMaterialAmbient,
    kMaterialDiffuse,
    kMaterialSpecular,
    */
    kSemanticCount,
  };
public:
  static std::vector<ShaderNameBind> &GetPredefinedAttribList();
  static std::vector<ShaderNameBind> &GetPredefinedUniformList();
  static ShaderNameBind *GetPredefinedAttrib(int semantic);
  static ShaderNameBind *GetPredefinedUniform(int semantic);

public:
  ShaderBindPolicy();
  ShaderBindPolicy(GLuint shader_prog);
  ~ShaderBindPolicy();

  void set_var(int code, const ShaderVariable &v);
  const ShaderVariable &var(int code) const;
  GLuint shader_prog() const { return shader_prog_; }
  bool IsValid() const { return (shader_prog_ != 0); }

private:
  GLuint shader_prog_; //어떤 쉐이더를 위한 bind변수모음인가
  ShaderVariable var_list_[kSemanticCount];
};
}

#endif  // SORA_SHADER_BIND_POLICY_H_