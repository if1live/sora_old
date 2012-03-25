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
#include "core/template_lib.h"
#include "uber_shader.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include "camera.h"
#include "mesh.h"

namespace sora {;

GLuint Renderer::last_tex_id_ = -1;
GLuint Renderer::last_prog_id_ = -1;
ShaderProgram *Renderer::last_prog_ = NULL;

ShaderBindPolicy Renderer::bind_policy_;

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
  if(win_width_ != w || win_height_ != h) {
    LOGI("setupGraphics(%d, %d)", (int)w, (int)h);

    win_width_ = w;
    win_height_ = h;

    glViewport(0, 0, (int)w, (int)h);
    GLHelper::CheckError("glViewport");
  }
}

void Renderer::SetTexture(const Texture &tex) {
  if(last_tex_id_ != tex.handle()) {
    glBindTexture(GL_TEXTURE_2D, tex.handle());
    last_tex_id_ = tex.handle();
  }
}

void Renderer::SetShader(const ShaderProgram &prog, const ShaderBindPolicy &policy) {
  if(last_prog_id_ != prog.prog) {
    glUseProgram(prog.prog);
    last_prog_id_ = prog.prog;
    last_prog_ = const_cast<ShaderProgram*>(&prog);
  }
  bind_policy_ = policy;
}

void Renderer::SetMaterial(const Material &material) {
  SR_ASSERT(last_prog_ != NULL);

  const ShaderVariable &ambient_var = bind_policy_.var(ShaderBindPolicy::kAmbientColor);
  if(ambient_var.location != -1) {
    float color[4];
    memcpy(color, material.ambient, sizeof(material.ambient));
    //color[3] = material.alpha;
    color[3] = 1.0f;
    glUniform4fv(ambient_var.location, 1, color);
    GLHelper::CheckError("Uniform AmbientColor");
  }

  const ShaderVariable &diffuse_var = bind_policy_.var(ShaderBindPolicy::kDiffuseColor);
  if(diffuse_var.location != -1) {
    float color[4];
    memcpy(color, material.diffuse, sizeof(material.diffuse));
    //color[3] = material.alpha;
    color[3] = 1.0f;
    glUniform4fv(diffuse_var.location, 1, color);
    GLHelper::CheckError("Uniform DiffuseColor");
  }

  //specular
  if(material.illumination_model == 1) {
    //not use ks
    ;
  } else if(material.illumination_model == 2) {
    //use ks, specular
    const ShaderVariable &specular_var = bind_policy_.var(ShaderBindPolicy::kSpecularColor);
    if(specular_var.location != -1) {
      float color[4];
      memcpy(color, material.specular, sizeof(material.specular));
      //color[3] = material.alpha;
      color[3] = 1.0f;
      glUniform4fv(specular_var.location, 1, color);
      GLHelper::CheckError("Uniform SpecularColor");
    }

    const ShaderVariable &shininess_var = bind_policy_.var(ShaderBindPolicy::kSpecularShininess);
    if(shininess_var.location != -1) {
      glUniform1f(shininess_var.location, material.shininess);
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

void Renderer::Draw(const DrawCommand &cmd) {
  if(cmd.vert_ptr == NULL) { return; }
  if(cmd.index_ptr == NULL) { return; }
  if(cmd.index_count == 0) { return; }

  const ShaderVariable &pos_var = bind_policy_.var(ShaderBindPolicy::kPosition);
  const ShaderVariable &texcoord_var = bind_policy_.var(ShaderBindPolicy::kTexcoord);
  const ShaderVariable &normal_var = bind_policy_.var(ShaderBindPolicy::kNormal);

  const Vertex *vert_ptr = cmd.vert_ptr;

  //draw cube
  if(pos_var.location != -1) {
    glVertexAttribPointer(pos_var.location, 3, Vertex::kPosType, GL_FALSE, sizeof(sora::Vertex), &vert_ptr->pos);
  }
  if(texcoord_var.location != 1) {
    glVertexAttribPointer(texcoord_var.location, 2, Vertex::kTexcoordType, GL_FALSE, sizeof(sora::Vertex), &vert_ptr->texcoord);
  }
  if(normal_var.location != -1) {
    glVertexAttribPointer(normal_var.location, 3, Vertex::kNormalType, GL_FALSE, sizeof(sora::Vertex), &vert_ptr->normal);
  }

  int index_count = cmd.index_count;
  GLenum draw_mode = cmd.draw_mode;
  GLenum index_type = cmd.index_type;
  const void *index_ptr = cmd.index_ptr;
  glDrawElements(draw_mode, index_count, index_type, index_ptr);
}

void Renderer::Draw(const MeshBufferObject &mesh) {
  const ShaderVariable &pos_var = bind_policy_.var(ShaderBindPolicy::kPosition);
  const ShaderVariable &texcoord_var = bind_policy_.var(ShaderBindPolicy::kTexcoord);
  const ShaderVariable &normal_var = bind_policy_.var(ShaderBindPolicy::kNormal);

  for(int i = 0 ; i < mesh.BufferCount() ; i++) {
    const VertexBufferObject &vbo = mesh.vbo(i);
    const IndexBufferObject &ibo = mesh.ibo(i);
    int index_count = mesh.index_count(i);
    GLenum draw_mode = mesh.draw_mode(i);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.buffer());

    if(pos_var.location != -1) {
      int offset = offsetof(Vertex, pos);
      glVertexAttribPointer(pos_var.location, 3, Vertex::kPosType, GL_FALSE, sizeof(sora::Vertex), (void*)offset);
    }
    if(texcoord_var.location != 1) {
      int offset = offsetof(Vertex, texcoord);
      glVertexAttribPointer(texcoord_var.location, 2, Vertex::kTexcoordType, GL_FALSE, sizeof(sora::Vertex), (void*)offset);
    }
    if(normal_var.location != -1) {
      int offset = offsetof(Vertex, normal);
      glVertexAttribPointer(normal_var.location, 3, Vertex::kNormalType, GL_FALSE, sizeof(sora::Vertex), (void*)offset);
    }
    glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, 0);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
  const ShaderVariable &mvp_var = bind_policy_.var(ShaderBindPolicy::kWorldViewProjection);
  if(mvp_var.location != -1) {
    //glm::mat4 mvp = impl->projection * impl->view * impl->world;  
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= projection_mat();
    mvp *= view_mat();
    mvp *= world_mat;
    glUniformMatrix4fv(mvp_var.location, 1, GL_FALSE, glm::value_ptr(mvp));
  }

  const ShaderVariable &world_var = bind_policy_.var(ShaderBindPolicy::kWorld);
  if(world_var.location != -1) {
    glUniformMatrix4fv(world_var.location, 1, GL_FALSE, glm::value_ptr(world_mat));
  }

  const ShaderVariable &view_var = bind_policy_.var(ShaderBindPolicy::kView);
  if(view_var.location != -1) {
    glUniformMatrix4fv(view_var.location, 1, GL_FALSE, glm::value_ptr(view_mat()));
  }

  const ShaderVariable &projection_var = bind_policy_.var(ShaderBindPolicy::kProjection);
  if(projection_var.location != -1) {
    glUniformMatrix4fv(projection_var.location, 1, GL_FALSE, glm::value_ptr(projection_mat()));
  }

  const ShaderVariable &view_pos_handle = bind_policy_.var(ShaderBindPolicy::kViewPosition);
  if(view_pos_handle.location != -1) {
    glUniform4f(view_pos_handle.location, eye.x, eye.y, eye.z, 1.0f);
  }

  const ShaderVariable &view_side_var = bind_policy_.var(ShaderBindPolicy::kViewSide);
  if(view_side_var.location != -1) {
    Vec3f view_side;
    VecCross(dir.data, up.data, view_side.data);
    glUniform4f(view_side_var.location, view_side.x, view_side.y, view_side.z, 1.0f);
  }

  const ShaderVariable &view_up_var = bind_policy_.var(ShaderBindPolicy::kViewUp);
  if(view_up_var.location != -1) {
    glUniform4f(view_up_var.location, up.x, up.y, up.z, 1.0f);
  }

  const ShaderVariable &view_dir_var = bind_policy_.var(ShaderBindPolicy::kViewDirection);
  if(view_dir_var.location != -1) {
    glUniform4f(view_dir_var.location, dir.x, dir.y, dir.z, 1.0f);
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
  glDisable(GL_BLEND);
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
