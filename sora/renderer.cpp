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

struct RendererImpl {
  RendererImpl()
  : last_tex_id(-1),
    last_prog_id(-1),
    last_prog(NULL),
  projection(1.0f), view(1.0f), world(1.0f) {
  }
  //like cache?
  GLuint last_tex_id;
  GLuint last_prog_id;
  ShaderProgram *last_prog;

  //matrix
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 world;

  Camera cam;
};

Renderer::Renderer() 
: win_width_(480), 
win_height_(320), impl(new RendererImpl()) {
}
Renderer::~Renderer() {
  SafeDeleteWithNullCheck(impl);
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //reset matrix
  impl->projection = glm::ortho(0.0f, (float)win_width_, 0.0f, (float)win_height_);
  impl->view = glm::mat4(1.0f);
  impl->world = glm::mat4(1.0f);
}

void Renderer::Set3D() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  //glDisable(GL_DEPTH_TEST);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::SetTexture(const Texture &tex) {
  if(impl->last_tex_id != tex.handle()) {
    glBindTexture(GL_TEXTURE_2D, tex.handle());
    impl->last_tex_id = tex.handle();
  }
}

void Renderer::SetShader(const ShaderProgram &prog) {
  if(impl->last_prog_id != prog.prog) {
    glUseProgram(prog.prog);
    impl->last_prog_id = prog.prog;
    impl->last_prog = const_cast<ShaderProgram*>(&prog);
  }
}

void Renderer::SetMaterial(const Material &material) {
  SR_ASSERT(impl->last_prog != NULL);

  int ambient_color_loc = impl->last_prog->GetLocation(kLocationAmbientColor);
  if(ambient_color_loc != -1) {
    //float color[4];
    //memcpy(color, material.ambient, sizeof(material.ambient));
    //color[3] = material.alpha;
    glUniform4f(ambient_color_loc, material.ambient[0], material.ambient[1], material.ambient[2], material.alpha);
    GLHelper::CheckError("Uniform AmbientColor");
  }

  int diffuse_color_loc = impl->last_prog->GetLocation(kLocationDiffuseColor);
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
    int specular_color_loc = impl->last_prog->GetLocation(kLocationSpecularColor);
    if(specular_color_loc != -1) {
      float color[4];
      memcpy(color, material.specular, sizeof(material.specular));
      color[3] = material.alpha;
      glUniform4fv(specular_color_loc, 1, color);
      GLHelper::CheckError("Uniform SpecularColor");
    }
    int shininess_loc = impl->last_prog->GetLocation(kLocationSpecularShininess);
    if(shininess_loc != -1) {
      glUniform1f(shininess_loc, material.shininess);
      GLHelper::CheckError("Uniform Shininess");
    }
  } else {
    SR_ASSERT(!"not support yet");
  }
}

void Renderer::EndRender() {
  impl->last_tex_id = -1;
  impl->last_prog_id = -1;
  impl->last_prog = NULL;

  GLHelper::CheckError("EndRender");
}

void Renderer::DrawObj(const ObjModel &model) {
  int pos_loc = impl->last_prog->GetLocation(ShaderVariable::kPosition);
  int texcoord_loc = impl->last_prog->GetLocation(ShaderVariable::kTexcoord);
  int normal_loc = impl->last_prog->GetLocation(ShaderVariable::kNormal);

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
  int pos_loc = impl->last_prog->GetLocation(ShaderVariable::kPosition);
  int texcoord_loc = impl->last_prog->GetLocation(ShaderVariable::kTexcoord);
  int normal_loc = impl->last_prog->GetLocation(ShaderVariable::kNormal);

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
      glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &vert_ptr->normal);
    }
    //glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
    glDrawElements(draw_mode, idx_count, GL_UNSIGNED_SHORT, idx_ptr);
    GLHelper::CheckError("DrawElements - primitive model");
  }
}

glm::mat4 &Renderer::world_mat() {
  return impl->world;
}
glm::mat4 &Renderer::projection_mat() {
  return impl->projection;
}
glm::mat4 &Renderer::view_mat() {
  return impl->view;
}
void Renderer::set_world_mat(const glm::mat4 &m) {
  impl->world = m;
}
void Renderer::set_projection_mat(const glm::mat4 &m) {
  impl->projection = m;
}
void Renderer::set_view_mat(const glm::mat4 &m) {
  impl->view = m;
}

void Renderer::ApplyMatrix2D() {
  //world-view-projection
  //world, view, projection 같은것을 등록할수 잇으면 등록하기
  int mvp_handle = impl->last_prog->GetLocation(ShaderVariable::kWorldViewProjection);
  if(mvp_handle != -1) {
    //glm::mat4 mvp = impl->projection * impl->view * impl->world;  
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= impl->projection;
    mvp *= impl->view;
    mvp *= impl->world;
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, glm::value_ptr(mvp));
  }

  int world_handle = impl->last_prog->GetLocation(ShaderVariable::kWorld);
  if(world_handle != -1) {
    glUniformMatrix4fv(world_handle, 1, GL_FALSE, glm::value_ptr(impl->world));
  }

  int view_handle = impl->last_prog->GetLocation(ShaderVariable::kView);
  if(view_handle != -1) {
    glUniformMatrix4fv(view_handle, 1, GL_FALSE, glm::value_ptr(impl->view));
  }

  int projection_handle = impl->last_prog->GetLocation(ShaderVariable::kProjection);
  if(projection_handle != -1) {
    glUniformMatrix4fv(projection_handle, 1, GL_FALSE, glm::value_ptr(impl->projection));
  }
}

void Renderer::ApplyMatrix() {
  Camera &cam = impl->cam;
  //apply new viw matrix
  const Vec3f &eye = cam.eye();
  const Vec3f &dir = cam.dir();
  const Vec3f center = eye + dir;
  const Vec3f &up = cam.up();

  glm::vec3 eye_v(eye.x, eye.y, eye.z);
  glm::vec3 center_v(center.x, center.y, center.z);
  glm::vec3 up_v(up.x, up.y, up.z);

  glm::mat4 &view = impl->view;
  view = glm::lookAt(eye_v, center_v, up_v);

  //world-view-projection
  //world, view, projection 같은것을 등록할수 잇으면 등록하기
  int mvp_handle = impl->last_prog->GetLocation(ShaderVariable::kWorldViewProjection);
  if(mvp_handle != -1) {
    //glm::mat4 mvp = impl->projection * impl->view * impl->world;  
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= impl->projection;
    mvp *= impl->view;
    mvp *= impl->world;
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, glm::value_ptr(mvp));
  }

  int world_handle = impl->last_prog->GetLocation(ShaderVariable::kWorld);
  if(world_handle != -1) {
    glUniformMatrix4fv(world_handle, 1, GL_FALSE, glm::value_ptr(impl->world));
  }

  int view_handle = impl->last_prog->GetLocation(ShaderVariable::kView);
  if(view_handle != -1) {
    glUniformMatrix4fv(view_handle, 1, GL_FALSE, glm::value_ptr(impl->view));
  }

  int projection_handle = impl->last_prog->GetLocation(ShaderVariable::kProjection);
  if(projection_handle != -1) {
    glUniformMatrix4fv(projection_handle, 1, GL_FALSE, glm::value_ptr(impl->projection));
  }

  int view_pos_handle = impl->last_prog->GetLocation(ShaderVariable::kViewPosition);
  if(view_pos_handle != -1) {
    glUniform4f(view_pos_handle, eye.x, eye.y, eye.z, 1.0f);
  }

  int view_side_handle = impl->last_prog->GetLocation(ShaderVariable::kViewSide);
  if(view_side_handle != -1) {
    Vec3f view_side;
    VecCross(dir.data, up.data, view_side.data);
    glUniform4f(view_side_handle, view_side.x, view_side.y, view_side.z, 1.0f);
  }

  int view_up_handle = impl->last_prog->GetLocation(ShaderVariable::kViewUp);
  if(view_up_handle != -1) {
    glUniform4f(view_up_handle, up.x, up.y, up.z, 1.0f);
  }

  int view_dir_handle = impl->last_prog->GetLocation(ShaderVariable::kViewDirection);
  if(view_dir_handle != -1) {
    glUniform4f(view_dir_handle, dir.x, dir.y, dir.z, 1.0f);
  }
}

SR_C_DLL Renderer &Renderer_get_instance() {
  return Renderer::GetInstance();
}

Camera &Renderer::camera() {
  return impl->cam;
}
void Renderer::set_camera(const Camera &cam) {
  impl->cam = cam;

  //apply new viw matrix
  const Vec3f &eye = cam.eye();
  const Vec3f &dir = cam.dir();
  const Vec3f center = eye + dir;
  const Vec3f &up = cam.up();

  glm::vec3 eye_v(eye.x, eye.y, eye.z);
  glm::vec3 center_v(center.x, center.y, center.z);
  glm::vec3 up_v(up.x, up.y, up.z);

  glm::mat4 &view = impl->view;
  view = glm::lookAt(eye_v, center_v, up_v);
}
}
