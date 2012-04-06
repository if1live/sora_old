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
#include "shader_bind_policy.h"

using namespace std;

namespace sora {;

ShaderBindPolicy::ShaderBindPolicy(GLuint shader_prog)
: shader_prog_(shader_prog) {
}
ShaderBindPolicy::ShaderBindPolicy()
: shader_prog_(0) {
}
ShaderBindPolicy::~ShaderBindPolicy() {
}

void ShaderBindPolicy::set_var(int code, const ShaderVariable &v) {
  SR_ASSERT(code >= 0);
  SR_ASSERT(code < kSemanticCount);
  var_list_[code] = v;
}
const ShaderVariable &ShaderBindPolicy::var(int code) const {
  SR_ASSERT(code >= 0);
  SR_ASSERT(code < kSemanticCount);
  return var_list_[code];
}

std::vector<ShaderNameBind> &ShaderBindPolicy::GetPredefinedAttribList() {
  static vector<ShaderNameBind> attr_bind_param;
  if(attr_bind_param.empty()) {
    attr_bind_param.push_back(ShaderNameBind("a_position", ShaderBindPolicy::kPosition));
    attr_bind_param.push_back(ShaderNameBind("a_texcoord", ShaderBindPolicy::kTexcoord));
    attr_bind_param.push_back(ShaderNameBind("a_normal", ShaderBindPolicy::kNormal));
    attr_bind_param.push_back(ShaderNameBind("a_color", ShaderBindPolicy::kColor));
  }
  return attr_bind_param;
}
std::vector<ShaderNameBind> &ShaderBindPolicy::GetPredefinedUniformList() {
  static vector<ShaderNameBind> uniform_bind_param;
  if(uniform_bind_param.empty()) {
    uniform_bind_param.push_back(ShaderNameBind("u_constColor", ShaderBindPolicy::kConstColor));
    uniform_bind_param.push_back(ShaderNameBind("u_viewPosition", ShaderBindPolicy::kViewPosition));
    uniform_bind_param.push_back(ShaderNameBind("u_viewSide", ShaderBindPolicy::kViewSide));
    uniform_bind_param.push_back(ShaderNameBind("u_viewUp", ShaderBindPolicy::kViewUp));
    uniform_bind_param.push_back(ShaderNameBind("u_viewDir", ShaderBindPolicy::kViewDirection));
    uniform_bind_param.push_back(ShaderNameBind("u_worldViewProjection", ShaderBindPolicy::kWorldViewProjection));
    uniform_bind_param.push_back(ShaderNameBind("u_world", ShaderBindPolicy::kWorld));
    uniform_bind_param.push_back(ShaderNameBind("u_projection", ShaderBindPolicy::kProjection));
    uniform_bind_param.push_back(ShaderNameBind("u_view", ShaderBindPolicy::kView));

    uniform_bind_param.push_back(ShaderNameBind("u_ambientColor", ShaderBindPolicy::kAmbientColor));
    uniform_bind_param.push_back(ShaderNameBind("u_diffuseColor", ShaderBindPolicy::kDiffuseColor));
    uniform_bind_param.push_back(ShaderNameBind("u_specularColor", ShaderBindPolicy::kSpecularColor));
    uniform_bind_param.push_back(ShaderNameBind("u_specularShininess", ShaderBindPolicy::kSpecularShininess));

    uniform_bind_param.push_back(ShaderNameBind("s_diffuseMap", ShaderBindPolicy::kDiffuseMap));
    uniform_bind_param.push_back(ShaderNameBind("s_specularMap", ShaderBindPolicy::kSpecularMap));
    uniform_bind_param.push_back(ShaderNameBind("s_ambientMap", ShaderBindPolicy::kAmbientMap));
    uniform_bind_param.push_back(ShaderNameBind("s_texture", ShaderBindPolicy::kAlbedo));

    uniform_bind_param.push_back(ShaderNameBind("u_worldLightPosition", ShaderBindPolicy::kLightPosition));
  }
  return uniform_bind_param;
}

ShaderNameBind *ShaderBindPolicy::GetPredefinedAttrib(int semantic) {
  auto it = GetPredefinedAttribList().begin();
  auto endit = GetPredefinedAttribList().end();
  for( ; it != endit ; ++it) {
    if(it->semantic == semantic) {
      return &(*it);
    }
  }
  return NULL;
}
ShaderNameBind *ShaderBindPolicy::GetPredefinedUniform(int semantic) {
  auto it = GetPredefinedUniformList().begin();
  auto endit = GetPredefinedUniformList().end();
  for( ; it != endit ; ++it) {
    if(it->semantic == semantic) {
      return &(*it);
    }
  }
  return NULL;
}
void ShaderBindPolicy::Clear() {
  ShaderVariable empty_var;
  auto it = var_list_.begin();
  auto endit = var_list_.end();
  for( ; it != endit ; ++it) {
    *it = empty_var;
  }
}
}
