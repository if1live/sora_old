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

#include "renderer/renderer.h"
#include "renderer/uber_shader.h"

#include "event/touch_device.h"
#include "event/touch_event.h"
#include "renderer/selection_buffer.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

using namespace std;
using namespace glm;

namespace sora {;
namespace selection {
  ShaderProgram simple_shader;

  float win_width = 0;
  float win_height = 0;
  float rot_deg = 0;

  //테스트용 모델 이름
  const char *kCube1 = "cube1";
  const char *kCube2 = "cube2";
  const char *kPlane = "plane";

  SelectionBuffer selection_buffer;

  void setup_graphics(sora::Device *dev, int w, int h) {
    win_width = (float)w;
    win_height = (float)h;

    
    //create shader
    {
      std::string app_vert_path = sora::Filesystem::GetAppPath("shader/simple.vs");
      std::string app_frag_path = sora::Filesystem::GetAppPath("shader/simple.fs");
      simple_shader.LoadFromFile(app_vert_path, app_frag_path);
    }
    
    {
      //쉐도우 테스트용 평면
      sora::PrimitiveModel primitive_model;
      //primitive_model.WirePlane(3, 0.2);
      primitive_model.SolidPlane(4);
      //primitive_model.WireAxis(4);
      //primitive_model.SolidCylinder(3, 4, 4);
      //primitive_model.SolidTeapot(3);
      //primitive_model.SolidSphere(2, 16, 16);
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
      GLHelper::CheckError("glDrawArrays");
    }
    {
      std::string tex_path = sora::Filesystem::GetAppPath("texture/sora.png");
      sora::MemoryFile tex_file(tex_path);
      tex_file.Open();
      Texture tex("sora");
      tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
      dev->texture_mgr().Add(tex);
    }
    //gl상태
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    GLHelper::CheckError("glInitEnd");
  }

  void DrawScene(sora::Device *dev, sora::ShaderProgram &shader) {
    Renderer &render3d = dev->render3d();
    render3d.SetShader(shader);
    
    render3d.SetMaterial(Material::GetInvalidMtl());
    render3d.ApplyMaterialLight();

    //set camera
    Camera cam;
    cam.eye = vec3(-2, 3, 5);
    cam.up = vec3(0, 1, 0);
    cam.center = vec3(0);
    render3d.set_camera(cam);

    glm::mat4 &projection = render3d.projection_mat();
    projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 30.0f);

    glm::mat4 model(1.0f);
    render3d.ApplyMatrix(model);

    vector<string> mesh_list;
    mesh_list.push_back(kCube1);  //0 : cube
    mesh_list.push_back(kPlane);  //1 : plane
    for(size_t i = 0 ; i < mesh_list.size() ; i++) {
      const char *name = mesh_list[i].c_str();
      MeshBufferObject *mesh_buffer = dev->mesh_mgr().Get(name);
      
      //selection buffer. selection일떄만 대입시키기
      int color_id_loc = shader.GetUniformLocation("u_colorId");
      if(color_id_loc != -1) {
        //color_id를 ivec4로 변환하기 위해서 char배열로 변환후 다시 조합하자
        int color_id[4];
        SelectionBuffer::IdToArray(i, color_id);
        glUniform4iv(color_id_loc, 1, color_id);
      }
      render3d.Draw(*mesh_buffer);
    }
    GLHelper::CheckError("glDrawArrays");
  }

  void draw_frame(sora::Device *dev) {
    //obj rendering
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, (int)win_width, (int)win_height);

    //unsigned int flag = 0;
    //flag |= UberShader::kAmbientColor;
    //flag |= UberShader::kAmbientMap;
    //DrawScene(dev, dev->uber_shader(flag));
    DrawScene(dev, simple_shader);
    
    //DrawScene(dev, *selection_buffer.shader());

    Renderer::EndRender();
    GLHelper::CheckError("glDrawArrays");
  }

  void update_frame(sora::Device *dev, float dt) {
    TouchEventQueue &touch_evt_queue = dev->touch_evt_queue();

#if SR_WIN && (SR_GLES == 0)
    static float elapsed_time = 0;
    static int elapsed_tick_count = 0;
    elapsed_tick_count++;
    elapsed_time += dt;
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
      DrawScene(dev, *selection_buffer.shader());

      int color_id = request.GetId();
      if(color_id != -1) {
        LOGI("ColorId : %d", color_id);
      }
    }
  }
}
}