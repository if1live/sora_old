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
#include "mesh/primitive_model.h"
#include "core/template_lib.h"
#include "uber_shader.h"
#include "texture_manager.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include "camera.h"
#include "mesh.h"
#include "sys/device.h"
#include "render_state.h"

namespace sora {;

GLuint Renderer::last_tex_id_ = -1;
ShaderProgram *Renderer::last_prog_ = NULL;
Material Renderer::last_material_ = Material::GetInvalidMtl();

void Renderer::SetPolicy_2D() {
  if(policy_->policy_type != kRenderPolicy2D) {
    SafeDelete(policy_);
    policy_ = new RendererPolicy_2D();
  }
}
void Renderer::SetPolicy_3D() {
  if(policy_->policy_type != kRenderPolicy3D) {
    SafeDelete(policy_);
    policy_ = new RendererPolicy_3D();
  }
}
Renderer::Renderer(Device *dev) 
: projection_(1.0f),
view_(1.0f),
policy_(new RendererPolicy_3D()),
dev_(dev) {
}

Renderer::~Renderer() {
  SafeDelete(policy_);
}

void Renderer::SetTexture(const Texture &tex) {
  //ambient 같은거용 표준 텍스쳐
  if(last_tex_id_ != tex.handle()) {
    glActiveTexture(GL_TEXTURE0);
    //glUniform1i(0, 0);  //first_texture, GL_TEXTURE0
    glBindTexture(GL_TEXTURE_2D, tex.handle());
    last_tex_id_ = tex.handle();
  }
}

void Renderer::SetShader(const ShaderProgram &prog) {
  if(last_prog_ == NULL || (last_prog_->prog != prog.prog)) {
    glUseProgram(prog.prog);
    last_prog_ = const_cast<ShaderProgram*>(&prog);
  }
}

void Renderer::SetLight(const Light &light) {
  light_ = light;
}

void Renderer::ApplyMaterialLight() {
  //렌더러에 저장된 빛재질을 적절히 적용
  SR_ASSERT(last_prog_ != NULL);

  const ShaderBindPolicy &bind_policy = last_prog_->bind_policy;
  const Light &light = light_;
  const Material &material = mtl_;

  //apply light pos
  const ShaderVariable &light_pos_var = bind_policy.var(ShaderBindPolicy::kLightPosition);
  if(light_pos_var.location != -1) {
    glUniform3fv(light_pos_var.location, 1, glm::value_ptr(light_.pos));
    GLHelper::CheckError("Set Light Pos Handle");
  }

  //ambient, diffuse, specular 계산하기
  //light * 재질정보를 합쳐서 쓴다
  const ShaderVariable &ambient_var = bind_policy.var(ShaderBindPolicy::kAmbientColor);
  const ShaderVariable &diffuse_var = bind_policy.var(ShaderBindPolicy::kDiffuseColor);
  const ShaderVariable &specular_var = bind_policy.var(ShaderBindPolicy::kSpecularColor);
  const ShaderVariable &shininess_var = bind_policy.var(ShaderBindPolicy::kSpecularShininess);
  const ShaderVariable &ambient_map_var = bind_policy.var(ShaderBindPolicy::kAmbientMap);
  const ShaderVariable &diffuse_map_var = bind_policy.var(ShaderBindPolicy::kDiffuseMap);
  const ShaderVariable &specular_map_var = bind_policy.var(ShaderBindPolicy::kSpecularMap);
  const ShaderVariable &normal_map_var = bind_policy.var(ShaderBindPolicy::kNormalMap);

  bool use_ambient = false;
  bool use_diffuse = false;
  bool use_specular = false;

  float ambient_color[4];
  float diffuse_color[4];
  float specular_color[4];
  //memset(ambient_color, 0, sizeof(ambient_color));
  //memset(diffuse_color, 0, sizeof(diffuse_color));
  //memset(specular_color, 0, sizeof(specular_color));

  //색 정보 얻기
  if(ambient_var.location != -1) {
    use_ambient = true;

    //material의 색속성
    memcpy(ambient_color, &material.ambient, sizeof(material.ambient));
    ambient_color[3] = 1.0f;
    //빛속성과 조합
    for(int i = 0 ; i < 4 ; ++i) {
      ambient_color[i] *= light.ambient[i];
    }
  }
  if(diffuse_var.location != -1) {
    use_diffuse = true;

    memcpy(diffuse_color, &material.diffuse, sizeof(material.diffuse));
    diffuse_color[3] = 1.0f;
    for(int i = 0 ; i < 4 ; ++i) {
      diffuse_color[i] *= light.diffuse[i];
    }
  }
  if(specular_var.location != -1) {
    use_specular = true;

    memcpy(specular_color, &material.specular, sizeof(material.specular));
    specular_color[3] = 1.0f;
    for(int i = 0 ; i < 4 ; ++i) {
      specular_color[i] *= light.specular[i];
    }
  }


  if(use_ambient) {
    glUniform4fv(ambient_var.location, 1, ambient_color);
    GLHelper::CheckError("Uniform AmbientColor");
  }
  if(use_diffuse) {
    glUniform4fv(diffuse_var.location, 1, diffuse_color);
    GLHelper::CheckError("Uniform DiffuseColor");
  }
  //specular
  if(use_specular) {
    glUniform4fv(specular_var.location, 1, specular_color);
    GLHelper::CheckError("Uniform SpecularColor");
  }
  if(shininess_var.location != -1) {
    glUniform1f(shininess_var.location, material.shininess);
    GLHelper::CheckError("Uniform Shininess");
  }

  if(last_material_ != mtl_) {
    //가장 마지막에 바인딩한것과 지금꺼가 다르다면, 아마도 내부속성 대부분이 다르다는 소리이다
    //단순 빛색깔 같은거는 큰 부하가 아니고 빛계산과 중복되서 들어가는걸
    //처리하기 귀찮으니까 마지막꺼과 비교없이 그냥 대입하자
    //하지만 texture의 경우는 부하가 좀 크기가 검사를 해서
    //바뀐 경우에만 변경하자
    last_material_ = mtl_;

    //ambient봐서 텍스쳐 교체하기
    if(use_ambient && ambient_map_var.location != -1) {
      Texture *ambient_map = dev_->texture_mgr().Get_ptr(mtl_.ambient_map);
      if(ambient_map != NULL) {
        SetTexture(*ambient_map);
        glUniform1i(ambient_map_var.location, 0);
      }
    }
    if(use_diffuse && diffuse_map_var.location != -1) {
      Texture *diffuse_map = dev_->texture_mgr().Get_ptr(mtl_.diffuse_map);
      if(diffuse_map != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuse_map->handle());
        glUniform1i(diffuse_map_var.location, 1);
      }
    }
    if(use_specular && specular_map_var.location != -1) {
      Texture *specular_map = dev_->texture_mgr().Get_ptr(mtl_.specular_map);
      if(specular_map != NULL) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specular_map->handle());
        glUniform1i(specular_map_var.location, 2);
      }
    }
    //normal잇으면 적절히 등록
    if((mtl_.uber_flag & UberShader::kNormalMap) && normal_map_var.location != -1) {
      Texture *normal_map = dev_->texture_mgr().Get_ptr(mtl_.normal_map);
      if(normal_map != NULL) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normal_map->handle());
        glUniform1i(normal_map_var.location, 3);
      }
    }
  }

  //최초 상태로 돌려놓기
  glActiveTexture(GL_TEXTURE0);
}
void Renderer::SetMaterial(const Material &material) {
  mtl_ = material;
}

void Renderer::EndRender() {
  last_tex_id_ = -1;
  last_prog_ = NULL;

  //마테리얼 대충 날리기
  last_material_.uber_flag = Material::kInvalidShaderFlag; //설마 알파 -1인 정상 재질이 있을리가 없지

  GLHelper::CheckError("EndRender");
}

Camera &Renderer::camera() {
  return cam_;
}
void Renderer::set_camera(const Camera &cam) {
  cam_ = cam;

  //apply new viw matrix
  glm::mat4 &view = view_mat();
  view = glm::lookAt(cam.eye, cam.center, cam.up);
}

void Renderer::Draw(const DrawCommand &cmd) {
  if(cmd.vert_ptr == NULL) { return; }
  if(cmd.index_ptr == NULL) { return; }
  if(cmd.index_count == 0) { return; }

  const ShaderBindPolicy &bind_policy = last_prog_->bind_policy;

  const ShaderVariable &pos_var = bind_policy.var(ShaderBindPolicy::kPosition);
  const ShaderVariable &texcoord_var = bind_policy.var(ShaderBindPolicy::kTexcoord);
  const ShaderVariable &normal_var = bind_policy.var(ShaderBindPolicy::kNormal);

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
  const ShaderBindPolicy &bind_policy = last_prog_->bind_policy;

  const ShaderVariable &pos_var = bind_policy.var(ShaderBindPolicy::kPosition);
  const ShaderVariable &texcoord_var = bind_policy.var(ShaderBindPolicy::kTexcoord);
  const ShaderVariable &normal_var = bind_policy.var(ShaderBindPolicy::kNormal);
  const ShaderVariable &color_var = bind_policy.var(ShaderBindPolicy::kColor);
  
  //tangent가 잇으면 쓰자
  const ShaderVariable &tangent_var = bind_policy.var(ShaderBindPolicy::kTangent);

  for(int i = 0 ; i < mesh.BufferCount() ; i++) {
    const VertexBufferObject &vbo = mesh.vbo(i);
    const IndexBufferObject &ibo = mesh.ibo(i);
    int index_count = mesh.index_count(i);
    GLenum draw_mode = mesh.draw_mode(i);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.buffer());

    if(pos_var.location != -1) {
      TangentVertex tmp_v;
      const void *ptr = &tmp_v.pos;
      const void *base = &tmp_v;
      int offset = (long)ptr - (long)base;

      glEnableVertexAttribArray(pos_var.location);
      glVertexAttribPointer(pos_var.location, 3, Vertex::kPosType, GL_FALSE, sizeof(TangentVertex), (void*)offset);
      GLHelper::CheckError("glVertexAttribPointer");
    }
    if(texcoord_var.location != -1) {
      TangentVertex tmp_v;
      const void *ptr = &tmp_v.texcoord;
      const void *base = &tmp_v;
      int offset = (long)ptr - (long)base;
      
      glEnableVertexAttribArray(texcoord_var.location);
      glVertexAttribPointer(texcoord_var.location, 2, Vertex::kTexcoordType, GL_FALSE, sizeof(TangentVertex), (void*)offset);
      GLHelper::CheckError("glVertexAttribPointer");
    }
    if(normal_var.location != -1) {
      TangentVertex tmp_v;
      const void *ptr = &tmp_v.normal;
      const void *base = &tmp_v;
      int offset = (long)ptr - (long)base;

      glEnableVertexAttribArray(normal_var.location);
      glVertexAttribPointer(normal_var.location, 3, Vertex::kNormalType, GL_FALSE, sizeof(TangentVertex), (void*)offset);
      GLHelper::CheckError("glVertexAttribPointer");
    }
    if(color_var.location != -1) {
      TangentVertex tmp_v;
      const void *ptr = &tmp_v.color;
      const void *base = &tmp_v;
      int offset = (long)ptr - (long)base;

      //색속성은 ubyte니까 normalize해야됨
      glEnableVertexAttribArray(color_var.location);
      glVertexAttribPointer(color_var.location, 4, Vertex::kColorType, GL_TRUE, sizeof(TangentVertex), (void*)offset);
      GLHelper::CheckError("glVertexAttribPointer");
    }
    if(tangent_var.location != -1) {
      TangentVertex tmp_v;
      const void *ptr = &tmp_v.tangent;
      const void *base = &tmp_v;
      int offset = (long)ptr - (long)base;

      glEnableVertexAttribArray(tangent_var.location);
      glVertexAttribPointer(tangent_var.location, 4, TangentVertex::kTangentType, GL_FALSE, sizeof(TangentVertex), (void*)offset);
      GLHelper::CheckError("glVertexAttribPointer");
    }

    glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, 0);
    GLHelper::CheckError("DrawElement");
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::ApplyMatrix(const glm::mat4 &world_mat) {
  const ShaderBindPolicy &bind_policy = last_prog_->bind_policy;

  const Camera &cam = camera();
  glm::mat4 &view = view_mat();
  float win_w = (float)dev_->render_state().win_width();
  float win_h = (float)dev_->render_state().win_height();
  view = policy_->ToViewMatrixFromCamera(cam, win_w, win_h);
  //apply new viw matrix

  //camera vector
  const glm::vec3 &eye = cam.eye;
  const glm::vec3 &center = cam.center;
  const glm::vec3 &up = cam.up;

  //world-view-projection
  //world, view, projection 같은것을 등록할수 잇으면 등록하기
  const ShaderVariable &mvp_var = bind_policy.var(ShaderBindPolicy::kWorldViewProjection);
  if(mvp_var.location != -1) {
    //glm::mat4 mvp = impl->projection * impl->view * impl->world;  
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= projection_mat();
    mvp *= view_mat();
    mvp *= world_mat;
    glUniformMatrix4fv(mvp_var.location, 1, GL_FALSE, glm::value_ptr(mvp));
  }

  const ShaderVariable &world_var = bind_policy.var(ShaderBindPolicy::kWorld);
  if(world_var.location != -1) {
    glUniformMatrix4fv(world_var.location, 1, GL_FALSE, glm::value_ptr(world_mat));
  }

  const ShaderVariable &view_var = bind_policy.var(ShaderBindPolicy::kView);
  if(view_var.location != -1) {
    glUniformMatrix4fv(view_var.location, 1, GL_FALSE, glm::value_ptr(view_mat()));
  }

  const ShaderVariable &projection_var = bind_policy.var(ShaderBindPolicy::kProjection);
  if(projection_var.location != -1) {
    glUniformMatrix4fv(projection_var.location, 1, GL_FALSE, glm::value_ptr(projection_mat()));
  }

  const ShaderVariable &view_pos_handle = bind_policy.var(ShaderBindPolicy::kViewPosition);
  if(view_pos_handle.location != -1) {
    glUniform4f(view_pos_handle.location, eye.x, eye.y, eye.z, 1.0f);
  }

  const ShaderVariable &view_side_var = bind_policy.var(ShaderBindPolicy::kViewSide);
  if(view_side_var.location != -1) {
    glm::vec3 dir = center - eye;
    glm::vec3 view_side = glm::cross(dir, up);
    glUniform4f(view_side_var.location, view_side.x, view_side.y, view_side.z, 1.0f);
  }

  const ShaderVariable &view_up_var = bind_policy.var(ShaderBindPolicy::kViewUp);
  if(view_up_var.location != -1) {
    glUniform4f(view_up_var.location, up.x, up.y, up.z, 1.0f);
  }
  /*
  const ShaderVariable &view_dir_var = bind_policy.var(ShaderBindPolicy::kViewDirection);
  if(view_dir_var.location != -1) {
    glUniform4f(view_dir_var.location, dir.x, dir.y, dir.z, 1.0f);
  }
  */
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

glm::mat4 RendererPolicy_2D::ToViewMatrixFromCamera(const Camera &cam, float win_w, float win_h) {
  //TODO 2D renderer의 경우, 아직 cam속성을 갖다쓰지 않는다. 이부분을 적절히 고쳐주기
  //apply new viw matrix
  glm::mat4 view = glm::ortho(0.0f, win_w, 0.0f, win_h);
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

glm::mat4 RendererPolicy_3D::ToViewMatrixFromCamera(const Camera &cam, float win_w, float win_h) {
  //apply new viw matrix
  glm::mat4 view = glm::lookAt(cam.eye, cam.center, cam.up);
  return view;
}

}
