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
#include "sora_stdafx.h"
#include "uber_shader.h"
#include "core/template_lib.h"
#include "sys/filesystem.h"
#include "sys/memory_file.h"
#include "gl_helper.h"
#include "shader_bind_policy.h"

using namespace std;

namespace sora {;
//uber shader의 enum에 정의된 순서대로 맞춰서 쓴다
const char *enable_define_list[] = {
  "#define USE_CONST_COLOR 1\n",
  "#define USE_TEXTURE 1\n",
  "#define USE_AMBIENT 1\n",
  "#define USE_DIFFUSE 1\n",
  "#define USE_SPECULAR 1\n",
};
const char *disable_define_list[] = {
  "#undef USE_CONST_COLOR\n",
  "#undef USE_TEXTURE\n",
  "#undef USE_AMBIENT\n",
  "#undef USE_DIFFUSE\n",
  "#undef USE_SPECULAR\n",
};

UberShader::UberShader() {
  LoadRawSrc();
}

void UberShader::LoadRawSrc() {
  //uber shader 로딩
  std::string app_vert_path = Filesystem::GetAppPath("shader/v_uber.glsl");
  std::string app_frag_path = Filesystem::GetAppPath("shader/f_uber.glsl");
  MemoryFile vert_file(app_vert_path);
  MemoryFile frag_file(app_frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  orig_vert_src_ = vert_src;
  orig_frag_src_ = frag_src;
  vert_file.Close();
  frag_file.Close();
}

UberShader::~UberShader() {
  auto it = prog_dict_.begin();
  auto endit = prog_dict_.end();
  for( ; it != endit ; ++it) {
    it->second.Deinit();
  }
  prog_dict_.clear();
}

//테이블 구성해서 타자치는거 좀 줄이자
struct ShaderBindParam {
  ShaderBindParam(const char *name, int semantic)
    : name(name), semantic(semantic) {}
  string name;
  int semantic;
};

ShaderProgram &UberShader::Load(uint flag) {
  auto found = prog_dict_.find(flag);
  if(found != prog_dict_.end()) {
    return found->second;
  }
  //없으면 적절히 생성하기
  vector<const char*> vert_src_list;
  vector<const char*> frag_src_list;

  const int enable_count = sizeof(enable_define_list) / sizeof(enable_define_list[0]);
  const int disable_count = sizeof(disable_define_list) / sizeof(disable_define_list[0]);
  SR_STATIC_ASSERT(enable_count == disable_count, "must same size");
  for(int i = 0 ; i < enable_count ; i++) {
    unsigned int mask = 1 << i;
    if((flag & mask) == mask) {
      vert_src_list.push_back(enable_define_list[i]);
      frag_src_list.push_back(enable_define_list[i]);
    } else {
      vert_src_list.push_back(disable_define_list[i]);
      frag_src_list.push_back(disable_define_list[i]);
    }
  }
  vert_src_list.push_back(orig_vert_src_.c_str());
  frag_src_list.push_back(orig_frag_src_.c_str());

  //쉐이더 프로그램 적절히 생성
  ShaderProgram shader_prog;
  shader_prog.Init(vert_src_list, frag_src_list);

  ShaderBindPolicy &bind_policy = shader_prog.bind_policy;
  SR_ASSERT(bind_policy.shader_prog() == shader_prog.prog);

  //bind되는 변수는 uber shader의 경우는 코드레벨에서 떄려박을수 있다
  vector<ShaderBindParam> attr_bind_param;
  vector<ShaderBindParam> uniform_bind_param;
  
  uniform_bind_param.push_back(ShaderBindParam("u_constColor", ShaderBindPolicy::kConstColor));
  uniform_bind_param.push_back(ShaderBindParam("u_viewPosition", ShaderBindPolicy::kViewPosition));
  uniform_bind_param.push_back(ShaderBindParam("u_viewSide", ShaderBindPolicy::kViewSide));
  uniform_bind_param.push_back(ShaderBindParam("u_viewUp", ShaderBindPolicy::kViewUp));
  uniform_bind_param.push_back(ShaderBindParam("u_viewDir", ShaderBindPolicy::kViewDirection));
  uniform_bind_param.push_back(ShaderBindParam("u_worldViewProjection", ShaderBindPolicy::kWorldViewProjection));
  uniform_bind_param.push_back(ShaderBindParam("u_world", ShaderBindPolicy::kWorld));
  uniform_bind_param.push_back(ShaderBindParam("u_projection", ShaderBindPolicy::kProjection));
  uniform_bind_param.push_back(ShaderBindParam("u_view", ShaderBindPolicy::kView));
  uniform_bind_param.push_back(ShaderBindParam("u_ambientColor", ShaderBindPolicy::kAmbientColor));
  uniform_bind_param.push_back(ShaderBindParam("u_diffuseColor", ShaderBindPolicy::kDiffuseColor));
  uniform_bind_param.push_back(ShaderBindParam("u_specularColor", ShaderBindPolicy::kSpecularColor));
  uniform_bind_param.push_back(ShaderBindParam("u_specularShininess", ShaderBindPolicy::kSpecularShininess));
  uniform_bind_param.push_back(ShaderBindParam("u_worldLightPosition", ShaderBindPolicy::kLightPosition));

  attr_bind_param.push_back(ShaderBindParam("a_position", ShaderBindPolicy::kPosition));
  attr_bind_param.push_back(ShaderBindParam("a_texcoord", ShaderBindPolicy::kTexcoord));
  attr_bind_param.push_back(ShaderBindParam("a_normal", ShaderBindPolicy::kNormal));
  attr_bind_param.push_back(ShaderBindParam("a_color", ShaderBindPolicy::kColor));

  for(size_t i = 0 ; i < attr_bind_param.size() ; i++) {
    const ShaderBindParam &param = attr_bind_param[i];
    const ShaderVariable *var = shader_prog.attrib_var(param.name);
    if(var != NULL) {
      bind_policy.set_var(param.semantic, *var);
    }
  }
  for(size_t i = 0 ; i < uniform_bind_param.size() ; i++) {
    const ShaderBindParam &param = uniform_bind_param[i];
    const ShaderVariable *var = shader_prog.uniform_var(param.name);
    if(var != NULL) {
      bind_policy.set_var(param.semantic, *var);
    }
  }
  
  prog_dict_[flag] = shader_prog;
  return prog_dict_[flag];
}

}
