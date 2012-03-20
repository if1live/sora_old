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

Renderer &Renderer::Renderer3D() {
  static Renderer renderer(new RendererPolicy_3D());
  return renderer;
}
Renderer &Renderer::Renderer2D() {
  static Renderer renderer(new RendererPolicy_2D());
  return renderer;
}

Renderer::Renderer(RendererPolicy *policy) 
: projection_(1.0f),
view_(1.0f),
policy_(policy) {
}

Renderer::~Renderer() {
  SafeDelete(policy_);
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
    float color[4];
    memcpy(color, material.ambient, sizeof(material.ambient));
    //color[3] = material.alpha;
    color[3] = 1.0f;
    glUniform4fv(ambient_color_loc, 1, color);
    GLHelper::CheckError("Uniform AmbientColor");
  }

  int diffuse_color_loc = last_prog_->GetLocation(kLocationDiffuseColor);
  if(diffuse_color_loc != -1) {
    float color[4];
    memcpy(color, material.diffuse, sizeof(material.diffuse));
    //color[3] = material.alpha;
    color[3] = 1.0f;
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
      //color[3] = material.alpha;
      color[3] = 1.0f;
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

void Renderer::DrawObj(const ObjModel &model) {
  int pos_loc = last_prog()->GetLocation(ShaderVariable::kPosition);
  int texcoord_loc = last_prog()->GetLocation(ShaderVariable::kTexcoord);
  int normal_loc = last_prog()->GetLocation(ShaderVariable::kNormal);

  //draw cube
  if(pos_loc != -1) {
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &model.vertex_ptr()->pos);
  }
  if(texcoord_loc != 1) {
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &model.vertex_ptr()->texcoord);
  }
  if(normal_loc != -1) {
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &model.vertex_ptr()->normal);
  }
  glDrawElements(GL_TRIANGLES, model.index_count(), GL_UNSIGNED_SHORT, model.index_ptr());
}

void Renderer::DrawPrimitiveModel(const PrimitiveModel &model) {
  int pos_loc = last_prog()->GetLocation(ShaderVariable::kPosition);
  int texcoord_loc = last_prog()->GetLocation(ShaderVariable::kTexcoord);
  int normal_loc = last_prog()->GetLocation(ShaderVariable::kNormal);

  //draw primitive model
  for(int i = 0 ; i < model.Count() ; i++) {
    const sora::Vertex *vert_ptr = model.vertex_list(i);
    const void *idx_ptr = model.index_list(i);
    int idx_count = model.index_count(i);
    GLenum draw_mode = model.draw_mode(i);

    if(pos_loc != -1) {
      glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &vert_ptr->pos);
    }
    if(texcoord_loc != -1) {
      glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &vert_ptr->texcoord);
    }
    if(normal_loc != -1) {
      glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_TRUE, sizeof(sora::Vertex), &vert_ptr->normal);
    }
    //glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
    glDrawElements(draw_mode, idx_count, GL_UNSIGNED_SHORT, idx_ptr);
    GLHelper::CheckError("DrawElements - primitive model");
  }
}

void Renderer::ApplyMatrix(const glm::mat4 &world_mat) {
  const Camera &cam = camera();
  glm::mat4 &view = view_mat();
  view = policy_->ToViewMatrixFromCamera(cam);
  //apply new viw matrix

  //camera vector
  const Vec3f &eye = cam.eye();
  const Vec3f &dir = cam.dir();
  const Vec3f center = eye + dir;
  const Vec3f &up = cam.up();
  glm::vec3 eye_v(eye.x, eye.y, eye.z);
  glm::vec3 center_v(center.x, center.y, center.z);
  glm::vec3 up_v(up.x, up.y, up.z);

  //world-view-projection
  //world, view, projection 같은것을 등록할수 잇으면 등록하기
  int mvp_handle = last_prog()->GetLocation(ShaderVariable::kWorldViewProjection);
  if(mvp_handle != -1) {
    //glm::mat4 mvp = impl->projection * impl->view * impl->world;  
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= projection_mat();
    mvp *= view_mat();
    mvp *= world_mat;
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, glm::value_ptr(mvp));
  }

  int world_handle = last_prog()->GetLocation(ShaderVariable::kWorld);
  if(world_handle != -1) {
    glUniformMatrix4fv(world_handle, 1, GL_FALSE, glm::value_ptr(world_mat));
  }

  int view_handle = last_prog()->GetLocation(ShaderVariable::kView);
  if(view_handle != -1) {
    glUniformMatrix4fv(view_handle, 1, GL_FALSE, glm::value_ptr(view_mat()));
  }

  int projection_handle = last_prog()->GetLocation(ShaderVariable::kProjection);
  if(projection_handle != -1) {
    glUniformMatrix4fv(projection_handle, 1, GL_FALSE, glm::value_ptr(projection_mat()));
  }

  int view_pos_handle = last_prog()->GetLocation(ShaderVariable::kViewPosition);
  if(view_pos_handle != -1) {
    glUniform4f(view_pos_handle, eye.x, eye.y, eye.z, 1.0f);
  }

  int view_side_handle = last_prog()->GetLocation(ShaderVariable::kViewSide);
  if(view_side_handle != -1) {
    Vec3f view_side;
    VecCross(dir.data, up.data, view_side.data);
    glUniform4f(view_side_handle, view_side.x, view_side.y, view_side.z, 1.0f);
  }

  int view_up_handle = last_prog()->GetLocation(ShaderVariable::kViewUp);
  if(view_up_handle != -1) {
    glUniform4f(view_up_handle, up.x, up.y, up.z, 1.0f);
  }

  int view_dir_handle = last_prog()->GetLocation(ShaderVariable::kViewDirection);
  if(view_dir_handle != -1) {
    glUniform4f(view_dir_handle, dir.x, dir.y, dir.z, 1.0f);
  }
}

void Renderer::SetInitState() {
  policy_->SetInitState();
}

///////////////////////////////
void RendererPolicy_2D::SetInitState() {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

glm::mat4 RendererPolicy_2D::ToViewMatrixFromCamera(const Camera &cam) {
  //TODO 2D renderer의 경우, 아직 cam속성을 갖다쓰지 않는다. 이부분을 적절히 고쳐주기
  //apply new viw matrix
  float win_width = Renderer::win_width();
  float win_height = Renderer::win_height();
  glm::mat4 view = glm::ortho(0.0f, win_width, 0.0f, win_height);
  return view;
}

void RendererPolicy_3D::SetInitState() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  //glDisable(GL_DEPTH_TEST);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

glm::mat4 RendererPolicy_3D::ToViewMatrixFromCamera(const Camera &cam) {
  //apply new viw matrix
  const Vec3f &eye = cam.eye();
  const Vec3f &dir = cam.dir();
  const Vec3f center = eye + dir;
  const Vec3f &up = cam.up();

  glm::vec3 eye_v(eye.x, eye.y, eye.z);
  glm::vec3 center_v(center.x, center.y, center.z);
  glm::vec3 up_v(up.x, up.y, up.z);

  glm::mat4 view = glm::lookAt(eye_v, center_v, up_v);
  return view;
}

}
