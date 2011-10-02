﻿/*  Copyright (C) 2011 by if1live */
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
#include "precompile.h"

#include "runa/uber_shader.h"
#include "runa/shader_program.h"

using std::string;
using std::vector;

namespace runa {;
UberShader::UberShader(const std::string &vert_src, const std::string &frag_src)
  : orig_vert_src_(vert_src),
  orig_frag_src_(frag_src) {
}
UberShader::~UberShader() {
}

ShaderProgramPtr UberShader::getShader(FlagType flag) {
  ShaderDictType::iterator it = shader_dict_.find(flag);
  if (it == shader_dict_.end()) {
    // 코드 생성
    shader_dict_[flag] = create(flag);
  }
  return shader_dict_[flag];
}
ShaderProgramPtr UberShader::create(FlagType flag) const {
  string prefix = CreateMacroCode(flag);

  string vert_src = prefix + orig_vert_src_;
  string frag_src = prefix + orig_frag_src_;

  // MT_LINE_LOG("== vertex source ==");
  // MT_LINE_LOG(vert_src);
  // MT_LINE_LOG("== fragment source ==");
  // MT_LINE_LOG(frag_src);

  ShaderProgramPtr prog(new ShaderProgram(vert_src, frag_src));
  return prog;
}
std::string UberShader::CreateMacroCode(FlagType flag) const {
  string macro;
  if (HasFlag(flag, UBER_SHADER_USE_NORMAL)) {
    macro += "#define USE_NORMAL 1 \n";
  }
  if (HasFlag(flag, UBER_SHADER_USE_COLOR)) {
    macro += "#define USE_COLOR 1 \n";
  }
  if (HasFlag(flag, UBER_SHADER_USE_TEXCOORD)) {
    macro += "#define USE_TEXCOORD 1 \n";
  }
  return macro;
}
bool UberShader::HasFlag(FlagType src, FlagType test) const {
  return ((src & test) == test);
}
void UberShader::CleanupUnusedShader() {
  // MT_LOG("Uber Shader::shader count : %d", shader_dict_.size());

  vector<int> unused_list;

  ShaderDictType::iterator it = shader_dict_.begin();
  ShaderDictType::iterator endit = shader_dict_.end();
  for ( ; it != endit ; it++) {
    if (it->second.use_count() <= 1) {
      // 위의 tex와 it, 그래서 2면 그외의 아무곳에서도 참조하지 않는것이다
      unused_list.push_back(it->first);
    }
  }

  // unused texture의 실제 파기는 for뒤에서 한다(그래샤 loop가 문제없이 통과된다
  BOOST_FOREACH(int shader_id, unused_list) {
    ShaderDictType::iterator it = shader_dict_.find(shader_id);
    shader_dict_.erase(it);
  }

  // MT_LOG("Uber Shader::after cleanup unused
  // Shader count : %d", shader_dict_.size());
}
}
