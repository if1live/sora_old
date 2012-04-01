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
    kAmbientColor = 1 << 0,
    kDiffuseColor = 1 << 1,
    kSpecularColor = 1 << 2,
    kAmbientMap = 1 << 3,
    kDiffuseMap = 1 << 4,
    kSpecularMap = 1 << 5,
    //simple attribute
    kAlbedo = 1 << 6,
    kModelColor = 1 << 7,
    kConstColor = 1 << 8
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

//ambient / diffuse / specular 지원
//쌩텍스쳐는 곧 diffuse map라고 생각해서 연결한다
struct LightUberShaderLoadPolicy {
  static const char *vert_file() { return "shader/v_uber.glsl"; }
  static const char *frag_file() { return "shader/f_uber.glsl"; }
  static uint avail_mask() {
    uint flag = 0;
    flag |= UberShader::kAmbientColor;
    flag |= UberShader::kDiffuseColor;
    flag |= UberShader::kSpecularColor;
    flag |= UberShader::kAmbientMap;
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