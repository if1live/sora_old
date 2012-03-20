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
#include "renderer3d.h"

#include "primitive_model.h"
#include "obj_model.h"
#include "shader_variable.h"
#include "shader.h"
#include "gl_helper.h"

using namespace std;

namespace sora {;
Renderer3D::Renderer3D() {
}
Renderer3D::~Renderer3D() {
}

void Renderer3D::ApplyMatrix(const glm::mat4 &world_mat) {
  Camera &cam = camera();
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

void Renderer3D::DrawObj(const ObjModel &model) {
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
void Renderer3D::DrawPrimitiveModel(const PrimitiveModel &model) {
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

void Renderer3D::SetInitState() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  //glDisable(GL_DEPTH_TEST);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
}