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
#include "selection_main.h"

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

#include "event/touch_device.h"
#include "event/touch_event.h"
#include "renderer/selection_buffer.h"

using namespace std;
using namespace glm;

namespace sora {;
ShaderProgram selection_shader;
TextureManager tex_mgr;

ShaderProgram simple_shader;

float win_width = 0;
float win_height = 0;
float rot_deg = 0;

//테스트용 모델 이름
const char *kCube1 = "cube1";
const char *kCube2 = "cube2";
const char *kPlane = "plane";

SelectionBuffer selection_buffer;

void Selection_setup_graphics(sora::Device *dev, int w, int h) {
  win_width = w;
  win_height = h;

  
  //create shader
  {
    std::string app_vert_path = sora::Filesystem::GetAppPath("shader/selection_vert.glsl");
    std::string app_frag_path = sora::Filesystem::GetAppPath("shader/selection_frag.glsl");
    sora::MemoryFile vert_file(app_vert_path);
    sora::MemoryFile frag_file(app_frag_path);
    vert_file.Open();
    frag_file.Open();
    const char *vert_src = (const char*)(vert_file.start);
    const char *frag_src = (const char*)(frag_file.start);
    bool prog_result = selection_shader.Init(vert_src, frag_src);
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
    //init seleciton buf
    selection_buffer.Init(w, h);
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

void DrawScene(sora::Device *dev, sora::ShaderProgram &shader) {
  glUseProgram(shader.prog);

  int pos_loc = shader.GetAttribLocation("a_position");
  int tex_loc = simple_shader.GetAttribLocation("a_texcoord");
  glEnableVertexAttribArray(pos_loc);
  if(tex_loc != -1) {
    glEnableVertexAttribArray(tex_loc);
  }
  int mvp_loc = shader.GetUniformLocation("u_worldViewProjection");

  //set cam
  glm::vec3 eye(-2, 3, 5);
  glm::vec3 center(0);
  glm::vec3 up(0, 1, 0);
  glm::mat4 view = glm::lookAt(eye, center, up);
  glm::mat4 projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 30.0f);
  glm::mat4 model(1.0f);
  glm::mat4 mvp = projection * view * model;
  glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

  Texture *tex = tex_mgr.Get_ptr(string("sora"));
  glBindTexture(GL_TEXTURE_2D, tex->handle());

  vector<string> mesh_list;
  mesh_list.push_back(kCube1);  //0 : cube
  mesh_list.push_back(kPlane);  //1 : plane
  for(size_t i = 0 ; i < mesh_list.size() ; i++) {
    const char *name = mesh_list[i].c_str();
    MeshBufferObject *mesh_buffer = dev->mesh_mgr().Get(name);
    SR_ASSERT(mesh_buffer != NULL);

    int color_id_loc = shader.GetUniformLocation("u_colorId");
    if(color_id_loc != -1) {
      //color_id를 ivec4로 변환하기 위해서 char배열로 변환후 다시 조합하자
      int color_id[4];
      SelectionBuffer::IdToArray(i, color_id);
      glUniform4iv(color_id_loc, 1, color_id);
    }
  
    SR_ASSERT(mesh_buffer->BufferCount());
    GLuint vbo = mesh_buffer->vbo(0).buffer();
    GLuint ibo = mesh_buffer->ibo(0).buffer();
    int index_count = mesh_buffer->index_count(0);
    GLenum draw_mode = mesh_buffer->draw_mode(0);
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    if(tex_loc != -1) {
      glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
    }
    glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, 0);
      
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

void Selection_draw_frame(sora::Device *dev) {
  /*
  //select buffer rendering
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  DrawScene(dev, selection_shader);
  */
  //obj rendering
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, win_width, win_height);
  DrawScene(dev, simple_shader);

  GLHelper::CheckError("glDrawArrays");
}

void Selection_update_frame(sora::Device *dev, float dt) {
  static float elapsed_time = 0;
  static int elapsed_tick_count = 0;
  elapsed_tick_count++;
  elapsed_time += dt;
  TouchEventQueue &touch_evt_queue = dev->touch_evt_queue();

#if SR_WIN
  // touch event
  TouchDevice &touch_device = TouchDevice::GetInstance();
  //glfw 대응 소스
  int posx, posy;
  glfwGetMousePos(&posx, &posy);
  int left_state = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
  int left_btn_state = false;
  if(left_state == GLFW_RELEASE) {
    left_btn_state = kInputRelease;
  } else {
    left_btn_state = kInputPress;
  }
  touch_device.UpdateState(left_btn_state, posx, posy, elapsed_tick_count, elapsed_time);
  const vector<TouchEvent> &touch_evt_list = touch_device.GetCreatedEvent();
  auto touch_evt_it = touch_evt_list.begin();
  auto touch_evt_endit = touch_evt_list.end();
  for( ; touch_evt_it != touch_evt_endit ; ++touch_evt_it) {
    touch_evt_queue.Push(*touch_evt_it);
  }
#endif

  //터치한 좌표의 픽셀 얻기
  while(touch_evt_queue.IsEmpty() == false) {
    TouchEvent evt = touch_evt_queue.Get();
    int gl_x = evt.x;
    int gl_y = win_height - evt.y;

    //터치햇을때만 selection buffer만들어서 렌더링 부하를 줄이자
    SelectionRequest request(&selection_buffer, gl_x, gl_y);
    DrawScene(dev, selection_shader);

    int color_id = request.GetId();
    if(color_id != -1) {
      LOGI("ColorId : %d", color_id);
    }
  }
}

}