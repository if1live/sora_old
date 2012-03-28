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
#include "core/template_lib.h"

#if SR_USE_PCH == 0
#include "core/unordered_map_inc.h"
#endif

namespace sora {;
//ambient / diffuse / specular같은거 기본 지원하기 위한 용도
//설계 테스트용
class UberShader : public Singleton<UberShader> {
public:
  enum {
    kConstColor = 1 << 0,
    kTexture = 1 << 1,
    kAmbient = 1 << 2,
    kDiffuse = 1 << 3,
    kSpecular = 1 << 4,
  };
public:
  UberShader();
  ~UberShader();
  
  ShaderProgram &Load(uint flag);

private:
  void LoadRawSrc();

  typedef std::tr1::unordered_map<unsigned int, ShaderProgram> ShaderDictType;
  ShaderDictType prog_dict_;


  std::string orig_vert_src_;
  std::string orig_frag_src_;
};
}

#endif  // SORA_UBER_SHADER_H_