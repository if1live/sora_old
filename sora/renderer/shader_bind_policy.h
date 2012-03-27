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

class ShaderBindPolicy {
public:
  enum {
    kUnknown = -1,
    //predefined semantic list
    //vec4
    kViewDirection,
    kViewPosition,
    kViewSide,
    kViewUp,

    //mat4
    //kViewProjection,
    //kViewProjectionInverse,
    //kViewProjectionTranspose,
    //kViewProjectionInverseTranspose,

    kView,
    //kViewInverse,
    //kViewTranspose,
    //kViewInverseTrsnspose,

    kProjection,
    //kProjectionInverse,
    //kProjectionTranspose,
    //kProjectionInverseTranspose,

    kWorldViewProjection,
    //kWorldViewProjectionInverse,
    //kWorldViewProjectionTranspose,
    //kWorldViewProjectionInverseTranspose,

    kWorld,
    //kWorldInverse,
    //kWorldTranspose,
    //kWorldInverseTranspose,

    //kWorldView,
    //kWorldViewInverse,
    //kWorldViewTranspose,    
    //kWorldViewInverseTranspose,

    //position, normal...등등 항상 뻔한것들까지 일단 떄려박자
    kPosition,
    kNormal,
    kTexcoord,

    //기본적인 빛을 구현하는데 사용되는 속성
    kAmbientColor,
    kDiffuseColor,
    kSpecularColor,
    kSpecularShininess,
    kLightPosition,

    kSemanticCount,
  };

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