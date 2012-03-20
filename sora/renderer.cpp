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
#include "obj_model.h"
#include "primitive_model.h"
#include "template_lib.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include "camera.h"

namespace sora {;

GLuint Renderer::last_tex_id_ = -1;
GLuint Renderer::last_prog_id_ = -1;
ShaderProgram *Renderer::last_prog_ = NULL;

float Renderer::win_width_ = 480;
float Renderer::win_height_ = 320;

Renderer::Renderer() 
: projection_(1.0f),
view_(1.0f) {
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
  SR_ASSERT(last_prog_ != NULL);

  int ambient_color_loc = last_prog_->GetLocation(kLocationAmbientColor);
  if(ambient_color_loc != -1) {
    //float color[4];
    //memcpy(color, material.ambient, sizeof(material.ambient));
    //color[3] = material.alpha;
    glUniform4f(ambient_color_loc, material.ambient[0], material.ambient[1], material.ambient[2], material.alpha);
    GLHelper::CheckError("Uniform AmbientColor");
  }

  int diffuse_color_loc = last_prog_->GetLocation(kLocationDiffuseColor);
  if(diffuse_color_loc != -1) {
    float color[4];
    memcpy(color, material.diffuse, sizeof(material.diffuse));
    color[3] = material.alpha;
    glUniform4fv(diffuse_color_loc, 1, color);
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
      float color[4];
      memcpy(color, material.specular, sizeof(material.specular));
      color[3] = material.alpha;
      glUniform4fv(specular_color_loc, 1, color);
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

Camera &Renderer::camera() {
  return cam_;
}
void Renderer::set_camera(const Camera &cam) {
  cam_ = cam;

  //apply new viw matrix
  const Vec3f &eye = cam.eye();
  const Vec3f &dir = cam.dir();
  const Vec3f center = eye + dir;
  const Vec3f &up = cam.up();

  glm::vec3 eye_v(eye.x, eye.y, eye.z);
  glm::vec3 center_v(center.x, center.y, center.z);
  glm::vec3 up_v(up.x, up.y, up.z);

  glm::mat4 &view = view_mat();
  view = glm::lookAt(eye_v, center_v, up_v);
}
}
