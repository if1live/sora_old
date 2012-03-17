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
#include "renderer.h"

#include "texture.h"
#include "gl_helper.h"
#include "shader.h"
#include "material.h"

namespace sora {;
Renderer::Renderer() 
: win_width_(480), 
win_height_(320),
last_tex_id_(-1),
last_prog_id_(-1),
last_prog_(NULL) {
}
Renderer::~Renderer() {
}

void Renderer::SetWindowSize(float w, float h) {
  LOGI("setupGraphics(%d, %d)", (int)w, (int)h);

  win_width_ = w;
  win_height_ = h;

  LOGI("Version : %s", GLHelper::GetVersion().c_str());
  LOGI("Vendor : %s", GLHelper::GetVender().c_str());
  LOGI("Renderer : %s", GLHelper::GetRenderer().c_str());
  LOGI("Extensions : %s", GLHelper::GetExtensions().c_str());

  glViewport(0, 0, (int)w, (int)h);
  GLHelper::CheckError("glViewport");
}

void Renderer::Set2D() {
  glDisable(GL_DEPTH_TEST);
}

void Renderer::Set3D() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

void Renderer::SetTexture(const Texture &tex) {
  if(last_tex_id_ != tex.handle()) {
    glBindTexture(GL_TEXTURE_2D, tex.handle());
    last_tex_id_ = tex.handle();
  }
}

void Renderer::SetShader(const ShaderProgram &prog) {
  if(last_prog_id_ != prog.prog) {
    glUseProgram(prog.prog);
    last_prog_id_ = prog.prog;
    last_prog_ = const_cast<ShaderProgram*>(&prog);
  }
}

void Renderer::SetMaterial(const Material &material) {
  int ambient_color_loc = last_prog_->GetLocation(kLocationAmbientColor);
  if(ambient_color_loc != -1) {
    glUniform3fv(ambient_color_loc, 1, material.ambient);
    GLHelper::CheckError("Uniform AmbientColor");
  }

  int diffuse_color_loc = last_prog_->GetLocation(kLocationDiffuseColor);
  if(diffuse_color_loc != -1) {
    glUniform3fv(diffuse_color_loc, 1, material.diffuse);
    GLHelper::CheckError("Uniform DiffuseColor");
  }

  //specular
  if(material.illumination_model == 1) {
    //not use ks
    ;
  } else if(material.illumination_model == 2) {
    //use ks, specular
    int specular_color_loc = last_prog_->GetLocation(kLocationSpecularColor);
    if(specular_color_loc != -1) {
      glUniform3fv(specular_color_loc, 1, material.specular);
      GLHelper::CheckError("Uniform SpecularColor");
    }
    int shininess_loc = last_prog_->GetLocation(kLocationSpecularShininess);
    if(shininess_loc != -1) {
      glUniform1f(shininess_loc, material.shininess);
      GLHelper::CheckError("Uniform Shininess");
    }
  } else {
    SR_ASSERT(!"not support yet");
  }
}

void Renderer::EndRender() {
  last_tex_id_ = -1;
  last_prog_id_ = -1;
  last_prog_ = NULL;

  GLHelper::CheckError("EndRender");
}

SR_C_DLL Renderer &Renderer_get_instance() {
  return Renderer::GetInstance();
}
}
