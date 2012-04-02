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
#include "shadow_map_main.h"

#include "sys/filesystem.h"
#include "renderer/shader.h"
#include "sys/memory_file.h"
#include "renderer/texture.h"
#include "renderer/texture_manager.h"
#include "renderer/gl_helper.h"

#include "renderer/light.h"
#include "renderer/mesh_manager.h"
#include "sys/device.h"
#include "renderer/primitive_model.h"
#include "renderer/gl_buffer_object.h"

using namespace std;
using namespace glm;

namespace sora {;

ShaderProgram shader;
TextureManager tex_mgr;

float win_width = 0;
float win_height = 0;
float rot_deg = 0;

Light light;

//테스트용 모델 이름
const char *kCube1 = "cube1";
const char *kCube2 = "cube2";
const char *kPlane = "plane";

void ShadowMap_setup_graphics(sora::Device *dev, int w, int h) {
  win_width = w;
  win_height = h;

  //create shader
  //2d shader
  std::string app_vert_path = sora::Filesystem::GetAppPath("shader/shadow_map_vert.glsl");
  std::string app_frag_path = sora::Filesystem::GetAppPath("shader/shadow_map_frag.glsl");
  sora::MemoryFile vert_file(app_vert_path);
  sora::MemoryFile frag_file(app_frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  bool prog_result = shader.Init(vert_src, frag_src);
  if(prog_result == false) {
    LOGE("Could not create program.");
  }
  {
    //set light
    light.pos = Vec3f(2, 3, 5);
    light.up = Vec3f(0, 1, 0);
    light.dir = (-light.pos).Normalize();
  }
  {
    //쉐도우 테스트용 평면
    sora::PrimitiveModel primitive_model;
    primitive_model.SolidPlane(5);
    dev->mesh_mgr().Add(primitive_model.GetDrawCmdList(), kPlane);
  }
  {
    //쉐도우 테스트용 큐브
    sora::PrimitiveModel primitive_model;
    primitive_model.SolidCube(2, 2, 2, true);
    dev->mesh_mgr().Add(primitive_model.GetDrawCmdList(), kCube1);
  }

  //gl상태
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}
void ShadowMap_draw_frame(sora::Device *dev) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, win_width, win_height);

  GLHelper::CheckError("Render 2d start");

  int pos_loc = shader.GetAttribLocation("a_position");
  glEnableVertexAttribArray(pos_loc);
  int mvp_loc = shader.GetUniformLocation("u_worldViewProjection");

  glUseProgram(shader.prog);

  glm::vec3 eye(light.pos.x, light.pos.y, light.pos.z);
  glm::vec3 dir(light.dir.x, light.dir.y, light.dir.z);
  glm::vec3 up(light.up.x, light.up.y, light.up.z);
  glm::mat4 view = glm::lookAt(eye, eye + dir, up);
  float far_val = 30.0f;
  glm::mat4 projection = glm::perspective(45.0f, win_width / win_height, 0.1f, far_val);
  glm::mat4 mvp = projection * view;
  glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

  glm::mat3 world_mat(1.0f);
  int world_loc = shader.GetUniformLocation("u_world");
  glUniformMatrix3fv(world_loc, 1, GL_FALSE, glm::value_ptr(world_mat));

  int light_pos_loc = shader.GetUniformLocation("u_lightPosition");
  glUniform3fv(light_pos_loc, 1, glm::value_ptr(eye));

  int far_loc = shader.GetUniformLocation("u_far");
  glUniform1f(far_loc, far_val);


  vector<string> mesh_list;
  mesh_list.push_back(kCube1);
  mesh_list.push_back(kPlane);
  for(size_t i = 0 ; i < mesh_list.size() ; i++) {
    const char *name = mesh_list[i].c_str();
    MeshBufferObject *mesh_buffer = dev->mesh_mgr().Get(name);
    SR_ASSERT(mesh_buffer != NULL);
  
    SR_ASSERT(mesh_buffer->BufferCount());
    GLuint vbo = mesh_buffer->vbo(0).buffer();
    GLuint ibo = mesh_buffer->ibo(0).buffer();
    int index_count = mesh_buffer->index_count(0);
    GLenum draw_mode = mesh_buffer->draw_mode(0);
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, 0);
  }
  
  /*
  float vertex[] = {
    -0.5, -0.5, 0,
    0.5, -0.5, 0,
    0, 0.5, 0,
  };
  glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, vertex);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
  */

  GLHelper::CheckError("glDrawArrays");
}
void ShadowMap_update_frame(sora::Device *dev, float dt) {
  rot_deg += 0.1f;
}
}