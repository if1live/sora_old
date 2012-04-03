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
#include "renderer/camera.h"

using namespace std;
using namespace glm;

namespace sora {;

ShaderProgram shader;
TextureManager tex_mgr;

ShaderProgram simple_shader;

float win_width = 0;
float win_height = 0;
float rot_deg = 0;

Light light;
Camera cam;

//테스트용 모델 이름
const char *kCube1 = "cube1";
const char *kCube2 = "cube2";
const char *kPlane = "plane";

//shadow용 FBO만들기
struct FrameBufferObject {
  GLuint fbo;
  GLuint color;
  GLuint depth;
  int width;
  int height;

  //for render to texture
  GLuint img;
};
const float kShadowMapSacle = 1.0;
FrameBufferObject shadow_map_fbo;

void ShadowMap_setup_graphics(sora::Device *dev, int w, int h) {
  win_width = w;
  win_height = h;

  
  //create shader
  {
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
  }
  
  {
    std::string app_vert_path = sora::Filesystem::GetAppPath("shader/v_simple.glsl");
    std::string app_frag_path = sora::Filesystem::GetAppPath("shader/f_simple.glsl");
    sora::MemoryFile vert_file(app_vert_path);
    sora::MemoryFile frag_file(app_frag_path);
    vert_file.Open();
    frag_file.Open();
    const char *vert_src = (const char*)(vert_file.start);
    const char *frag_src = (const char*)(frag_file.start);
    bool prog_result = simple_shader.Init(vert_src, frag_src);
    if(prog_result == false) {
      LOGE("Could not create program.");
    }
  }
  {
    //set light
    light.pos = Vec3f(2, 3, 5);
    light.up = Vec3f(0, 1, 0);
    light.dir = (-light.pos).Normalize();
    
    cam.set_eye(Vec3f(-2, 3, 5));
    cam.set_up(Vec3f(0, 1, 0));
    cam.set_dir(light.dir);
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
  
  {
    //create shadow map fbo
    //깊이정보는 색정보에 섞어넣으니까 굳이 따로 만들 필요 없다
    shadow_map_fbo.width = w * kShadowMapSacle;
    shadow_map_fbo.height = h * kShadowMapSacle;
    FrameBufferObject &shadow_fbo = shadow_map_fbo;

    glGenFramebuffers(1, &shadow_map_fbo.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_fbo.fbo);

    //depth
    glGenRenderbuffers(1, &shadow_map_fbo.depth);
    glBindRenderbuffer(GL_RENDERBUFFER, shadow_map_fbo.depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, shadow_map_fbo.width, shadow_map_fbo.height);
    GLHelper::CheckError("glInitEnd");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadow_map_fbo.depth);
    GLHelper::CheckFrameBufferStatus("fb");

    glGenTextures(1, &shadow_map_fbo.img);
    glBindTexture(GL_TEXTURE_2D, shadow_map_fbo.img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, shadow_map_fbo.width, shadow_map_fbo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_map_fbo.img, 0);
    GLHelper::CheckFrameBufferStatus("fb");

    GLHelper::CheckError("glInitEnd");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  {
    std::string tex_path = sora::Filesystem::GetAppPath("texture/sora.png");
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex("sora");
    tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
    tex_mgr.Add(tex);
  }
  //gl상태
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  //GLHelper::CheckError("glInitEnd");
}
void ShadowMap_draw_frame(sora::Device *dev) {
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //fbo로 렌더링 하기
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_fbo.fbo);
  glViewport(0, 0, win_width, win_height);
  {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, win_width, win_height);

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
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  
  //쉐이더맵을 텍스쳐로 치고 렌더링 한번더 하기
  {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, win_width, win_height);
    glDisable(GL_DEPTH_TEST);

    GLHelper::CheckError("Render 2d start");
    //draw 2d something
    glm::mat4 world_mat(1.0f);

    int pos_loc = simple_shader.GetAttribLocation("a_position");
    int tex_loc = simple_shader.GetAttribLocation("a_texcoord");
    glEnableVertexAttribArray(pos_loc);
    glEnableVertexAttribArray(tex_loc);
  
    int mvp_loc = simple_shader.GetUniformLocation("u_worldViewProjection");

    float vertex[] = {
      -0.5, -0.5, 0,
      0.5, -0.5, 0,
      0.5, 0.5, 0,
      -0.5, 0.5, 0,
    };
    float texcoord[] = {
      0, 0,
      1, 0,
      1, 1,
      0, 1,
    };


    glUseProgram(simple_shader.prog);

    //Texture *tex = tex_mgr.Get_ptr(string("sora"));
    //glBindTexture(GL_TEXTURE_2D, tex->handle());
    glBindTexture(GL_TEXTURE_2D, shadow_map_fbo.img);

    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(world_mat));
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, vertex);
    glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    GLHelper::CheckError("glDrawArrays");
  }
  

  GLHelper::CheckError("glDrawArrays");
}
void ShadowMap_update_frame(sora::Device *dev, float dt) {
  rot_deg += 0.1f;
}
}