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

#if SR_USE_PCH == 0
#include "core/unordered_map_inc.h"
#endif

namespace sora {;

class UberShader {
public:
  enum {
    kConstColor = 1 << 0,
    kTexture = 1 << 1,
    kAmbientColor = 1 << 2,
    kDiffuseColor = 1 << 3,
    kSpecularColor = 1 << 4,
    kModelColor = 1 << 5,
    kDiffuseMap = 1 << 6,
    kSpecularMap = 1 << 7
  };
public:
  UberShader();
  virtual ~UberShader();

  template<typename T>
  void Init();
  ShaderProgram &Load(uint flag);

  bool IsValidFlag(uint flag) const;

protected:
  void LoadRawSrc(const std::string &v_file, const std::string &f_file);

  typedef std::tr1::unordered_map<unsigned int, ShaderProgram> ShaderDictType;
  ShaderDictType prog_dict_;

  std::string orig_vert_src_;
  std::string orig_frag_src_;
  uint avail_mask_;
};


//const color + model color + texture
//기본적인 디버깅이나 2d에서 사용하기 위해서 만들엇다
//이것을 둔 이유는
//빛계산은 색을 0으로 두고 시작해야하는데
//텍스쳐는 곱셈들어갈때 적절한 색이 있어야하기떄문이다
//사용가능한 플래그 : kConstColor | kTexture | kModelColor
struct SimpleUberShaderLoadPolicy {
  static const char *vert_file() { return "shader/v_simple_uber.glsl"; }
  static const char *frag_file() { return "shader/f_simple_uber.glsl"; }
  static uint avail_mask() {
    uint flag = 0;
    flag |= UberShader::kConstColor;
    flag |= UberShader::kTexture;
    flag |= UberShader::kModelColor;
    return flag;
  }
};

//ambient / diffuse / specular 지원
//쌩텍스쳐는 곧 diffuse map라고 생각해서 연결한다
struct LightUberShaderLoadPolicy {
  static const char *vert_file() { return "shader/v_light_uber.glsl"; }
  static const char *frag_file() { return "shader/f_light_uber.glsl"; }
  static uint avail_mask() {
    uint flag = 0;
    flag |= UberShader::kAmbientColor;
    flag |= UberShader::kDiffuseColor;
    flag |= UberShader::kSpecularColor;
    flag |= UberShader::kDiffuseMap;
    flag |= UberShader::kSpecularMap;
    return flag;
  }
};


template<typename T>
void UberShader::Init() {
  std::string vert_file(T::vert_file());
  std::string frag_file(T::frag_file());
  avail_mask_ = T::avail_mask();
  LoadRawSrc(vert_file, frag_file);
}

}

#endif  // SORA_UBER_SHADER_H_