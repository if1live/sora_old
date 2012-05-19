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
#include "shader_manager.h"
#include "string_hash.h"
#include "template_lib.h"

using namespace std;

const char *const_color_vert_src = "attribute vec4 a_position;\
uniform mat4 u_mvp;\
void main() {\
  gl_Position = u_mvp * a_position;\
}";

const char *const_color_frag_src = "precision mediump float;\
uniform vec4 u_constColor;\
void main() {\
  gl_FragColor = u_constColor;\
}";

const char *albedo_vert_src = "attribute vec4 a_position;\
attribute vec2 a_texcoord;\
uniform mat4 u_mvp;\
varying vec2 v_texcoord;\
void main() {\
  v_texcoord = a_texcoord;\
  gl_Position = u_mvp * a_position;\
}";

const char *albedo_frag_src = "precision mediump float;\
varying vec2 v_texcoord;\
uniform sampler2D s_texture;\
void main() {\
  gl_FragColor = texture2D(s_texture, v_texcoord);\
}";

const char *text_frag_src = "\
  precision mediump float;\
varying vec2 v_texcoord;\
uniform sampler2D s_texture;\
uniform vec4 u_constColor;\
void main() {\
	vec4 color = texture2D(s_texture, v_texcoord);\
  color.xyz = u_constColor.xyz; \
  gl_FragColor = color;\
}";

const char *vertex_color_vert_src = "attribute vec4 a_position;\
uniform mat4 u_mvp;\
attribute vec4 a_color;\
varying vec4 v_color;\
void main() {\
  v_color = a_color;\
  gl_Position = u_mvp * a_position;\
}";

const char *vertex_color_frag_src = "  precision mediump float;\
varying vec4 v_color; \
void main() {\
  gl_FragColor = v_color;\
}";

namespace sora {;
ShaderManager::ShaderManager() {
}
ShaderManager::~ShaderManager() {
  {
    auto it = shader_dict_.begin();
    auto endit = shader_dict_.end();
    for( ; it != endit ; ++it) {
      Shader &shader = it->second;
      shader.Deinit();
    }
  }
  {
    auto it = sys_shader_dict_.begin();
    auto endit = sys_shader_dict_.end();
    for( ; it != endit ; ++it) {
      Shader &shader = it->second;
      shader.Deinit();
    }
  }
}

void ShaderManager::SetUp() {  
}

Shader *ShaderManager::Get(SystemShaderType type) {
  //already exist..
  auto found = sys_shader_dict_.find(type);
  if(found != sys_shader_dict_.end()) {
    return &found->second;
  }

  struct SysShaderInitData {
    SystemShaderType type;
    const char *vert_src;
    const char *frag_src;
    const char *name;
  };

  SysShaderInitData init_data[] = {
    { kConstColor, const_color_vert_src, const_color_frag_src, "Const Color" },
    { kAlbedo, albedo_vert_src, albedo_frag_src, "Albedo" },
    { kVertexColor, vertex_color_vert_src, vertex_color_frag_src, "Vertex Color" },
    { kText, albedo_vert_src, text_frag_src, "Text" },
  };
  int data_size = GetArraySize(init_data);
  for(int i = 0 ; i < data_size ; ++i) {
    const SysShaderInitData &data = init_data[i];
    if(data.type == type) {
      Shader shader;
      LOGI("System Shader Name : %s", data.name);
      shader.Init(data.vert_src, data.frag_src);
      sys_shader_dict_[type] = shader;
      return &sys_shader_dict_[type];
    }
  }
  SR_ASSERT(!"not valid system shader");
  //not found..
  return NULL;
}

bool ShaderManager::Add(const Shader &shader, const std::string &name) {
  if(IsExist(name) == true) {
    return false;
  }
  uint hash = Hash(name);
  shader_dict_[hash] = shader;
  return true;
}
Shader *ShaderManager::Get(const std::string &name) {
  uint hash_code = Hash(name);
  return Get(hash_code);
}
Shader *ShaderManager::Get(unsigned int code) {
  auto found = shader_dict_.find(code);
  if(found == shader_dict_.end()) {
    return NULL;
  } else {
    return &(found->second);
  }
}
bool ShaderManager::IsExist(const std::string &name) const {
  uint hash_code = Hash(name);
  auto found = shader_dict_.find(hash_code);
  if(found == shader_dict_.end()) {
    return false;
  } else {
    return true;
  }
}
unsigned int ShaderManager::Hash(const std::string &str) {
  typedef RSHash HashClass;
  return HashClass::Hash(str);
}

bool ShaderManager::Add(const ShaderInitParam &param, const std::string &name) {
  Shader shader;
  if(param.init_type == ShaderInitParam::kShaderInitFromFile) {
    shader.LoadFromFile(param.vert, param.frag);
  } else if(param.init_type == ShaderInitParam::kShaderInitFromSrc) {
    shader.Init(param.vert, param.frag);
  } else {
    SR_ASSERT(!"not valid init type");
  }
  return Add(shader, name);
}
} //namespace sora
