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
#pragma once

#include "globals.h"
#include "shader.h"
#include "unordered_map_inc.h"

namespace sora {;

struct ShaderInitParam {
  typedef enum {
    kShaderInitFromFile,
    kShaderInitFromSrc
  } ShaderInitType;
  ShaderInitParam() : init_type(kShaderInitFromSrc) {}
  std::string vert;
  std::string frag;
  ShaderInitType init_type;
};

class ShaderManager {
public:
  //간단한 쉐이더 몇개는 시스템에 내장 시켜버리자
  //그러면 디버깅이나 그런 용도로 접근하는데 편하다
  typedef enum {
    kNone,
    kConstColor,
    kAlbedo,
    kVertexColor,
    kText,
  } SystemShaderType;
public:
  ShaderManager();
  ~ShaderManager();

  //manager 자체를 초기화하는거. 생성순서 통제와 연결되기떄문에 수동 초기화를 하도록햇다
  void SetUp();

  bool Add(const ShaderInitParam &param, const std::string &name);
  bool Add(const Shader &shader, const std::string &name);
  Shader *Get(const std::string &name);
  Shader *Get(unsigned int code);
  bool IsExist(const std::string &name) const;
  static unsigned int Hash(const std::string &str);

  //특수 행렬 몇개는 그냥 직접 얻을수 잇도록 햇다
  Shader *Get(SystemShaderType type);

private:
  typedef std::tr1::unordered_map<unsigned int, Shader> ShaderDictType;
  ShaderDictType shader_dict_;
  typedef std::map<SystemShaderType, Shader> SysShaderDictType;
  SysShaderDictType sys_shader_dict_;
};
} //namespace sora
