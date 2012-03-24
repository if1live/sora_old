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
#include "sora_main.h"

//android  OpenGL ES 2.0 code based
#if SR_USE_PCH == 0
#include "renderer/gl_inc.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#endif

#include "core/vector.h"
#include "renderer/gl_helper.h"
#include "sys/memory_file.h"

#include "renderer/shader.h"
#include "renderer/uber_shader.h"
#include "renderer/matrix_stack.h"

#include "renderer/obj_model.h"
#include "renderer/obj_loader.h"
#include "renderer/primitive_model.h"
#include "renderer/material_manager.h"

#include "renderer/texture.h"
#include "renderer/renderer.h"

#include "renderer/material.h"
#include "core/math_helper.h"
#include "renderer/camera.h"
#include "renderer/font.h"
#include "cbes/entity.h"
#include "cbes/world.h"
#include "cbes/mesh_component.h"
#include "cbes/component_list.h"
#include "renderer/texture_manager.h"

#include "renderer/shader_bind_policy.h"

#include "event/touch_device.h"
#include "event/touch_event.h"

using namespace std;
using namespace sora;
using namespace glm;

//ShaderProgram shader_prog;
ShaderProgram shader_2d;
//소멸자에서 만들엇던 gl을 해제하는데
//소멸자 호출 시점이 gl해제보다 늦어서 뻑난다. 그래서 임시로 동적할당해서 안놓음
UberShader &uber_shader = *(new UberShader());  

//cbes
World world;

void SORA_set_window_size(int w, int h) {
  sora::Renderer::SetWindowSize((float)w, (float)h);

}

#if SR_WIN
void SORA_test_draw(int w, int h) {
  static float a = 0;
  a += 0.1f;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  float vertex[] = {
    -0.5f, -0.5f,
    0.5f, -0.5f, 
    0.0f, 0.5f
  };
  unsigned char color[] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255,
  };

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(a, 0, 0, 1);

  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertex);
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
#endif

bool setupGraphics(int w, int h) {
  sora::Renderer::SetWindowSize((float)w, (float)h); 

  LOGI("Version : %s", GLHelper::GetVersion().c_str());
  LOGI("Vendor : %s", GLHelper::GetVender().c_str());
  LOGI("Renderer : %s", GLHelper::GetRenderer().c_str());
  LOGI("Extensions : %s", GLHelper::GetExtensions().c_str());

  uber_shader.Init();

  {
    //2d shader
    std::string app_vert_path = sora::Filesystem::GetAppPath("shader/v_simple.glsl");
    std::string app_frag_path = sora::Filesystem::GetAppPath("shader/f_simple.glsl");
    sora::MemoryFile vert_file(app_vert_path);
    sora::MemoryFile frag_file(app_frag_path);
    vert_file.Open();
    frag_file.Open();
    const char *vert_src = (const char*)(vert_file.start);
    const char *frag_src = (const char*)(frag_file.start);
    bool prog_result = shader_2d.Init(vert_src, frag_src);
    if(prog_result == false) {
      LOGE("Could not create program.");
      return false;
    }
  }

  //lodepng
  {
    std::string tex_path = sora::Filesystem::GetAppPath("texture/sora.png");
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex("sora");
    tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
    TextureManager::GetInstance().Add(tex);
  }
  {
    std::string tex_path = sora::Filesystem::GetAppPath("texture/sora2.png");
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex("sora2");
    tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
    TextureManager::GetInstance().Add(tex);
  }

  sora::ObjLoader loader;

  //load model
  std::string path1 = sora::Filesystem::GetAppPath("obj/cube.obj");
  sora::MemoryFile file1(path1);
  file1.Open();
  ObjModel obj_model;
  loader.LoadObj(file1.start, file1.end, &obj_model);

  //load material
  MaterialMgr_initialize_from_file();

  //primitive model test
  sora::PrimitiveModel primitive_model;
  //primitive_model.SolidCube(1, 2, 1, false);
  //primitive_model.WireCube(1, 1, 1);
  //primitive_model.WireAxis(2);
  //primitive_model.WireSphere(1, 8, 8);
  
  //primitive_model.WireCone(1, 2, 8, 8);
  //primitive_model.SolidCone(1, 2, 8, 8);
  //primitive_model.WireCylinder(1, 2, 8);
  //primitive_model.SolidCylinder(1, 2, 16);
  //primitive_model.SolidTeapot(2);
  //primitive_model.WireTeapot(1);

  //crate entity
  {
    Entity *entity_a = world.CreateEntity();
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    entity_mat = glm::rotate(glm::mat4(1.0f), 180.0f, vec3(1, 0, 0));
    entity_a->set_world_mat(entity_mat);

    MeshComponent *mesh_comp = new MeshComponent(entity_a);
    primitive_model.SolidSphere(0.5, 16, 16);
    mesh_comp->SetMesh(primitive_model);
    entity_a->AddComponent(mesh_comp);
    //mesh_comp->SetMesh(obj_model);
  }
  {
    Entity *entity_a = world.CreateEntity();
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    entity_mat = glm::translate(entity_mat, vec3(0.8, 0.3, 0));
    entity_mat = glm::rotate(entity_mat, 180.0f, vec3(1, 0, 0));
    entity_a->set_world_mat(entity_mat);
    
    MeshComponent *mesh_comp = new MeshComponent(entity_a);
    primitive_model.SolidCube(0.5, 0.5, 0.5, true);
    mesh_comp->SetMesh(primitive_model);
    entity_a->AddComponent(mesh_comp);
  }
  
  {
    Entity *entity_a = world.CreateEntity();
    glm::mat4 entity_mat = glm::mat4(1.0f);
    entity_mat = glm::translate(entity_mat, vec3(0, 0, -2.0f));
    entity_a->set_world_mat(entity_mat);
    
    MeshComponent *mesh_comp = new MeshComponent(entity_a);
    mesh_comp->SetMesh(obj_model);
    entity_a->AddComponent(mesh_comp);
  }

  return true;
}

float aptitude = 0; //위도. -90~90. 세로 위치 표현
float latitude = 0; //경도

void SORA_set_cam_pos(float a, float b) {
  aptitude += a;
  latitude += b;
  if(aptitude > 90) {
    aptitude = 90;
  } else if(aptitude < -90) {
    aptitude = -90;
  }
}

void renderFrame() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  /////////////////////////
  {
    Renderer &render3d = Renderer::Renderer3D();
    //render 3d
    render3d.SetInitState();

    //set camera + projection
    float win_width = render3d.win_width();
    float win_height = render3d.win_height();
    glm::mat4 &projection = render3d.projection_mat();
    projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);
    float radius = 4;
    float cam_x = radius * cos(SR_DEG_2_RAD(aptitude)) * sin(SR_DEG_2_RAD(latitude));
    float cam_y = radius * sin(SR_DEG_2_RAD(aptitude));
    float cam_z = radius * cos(SR_DEG_2_RAD(aptitude)) * cos(SR_DEG_2_RAD(latitude));

    sora::Camera cam;
    cam.set_eye(sora::Vec3f(cam_x, cam_y, cam_z));
    cam.set_dir(sora::Vec3f(0, 0, 0) - cam.eye());
    cam.set_up(sora::Vec3f(0, 1, 0));
    render3d.set_camera(cam);

    //shader 속성 설정
    ShaderBindPolicy bind_policy;
    //기본 속성
    const ShaderVariable &pos_var = uber_shader.prog().attrib_var("a_position");
    const ShaderVariable &texcoord_var = uber_shader.prog().attrib_var("a_texcoord");
    const ShaderVariable &normal_var = uber_shader.prog().attrib_var("a_normal");

    glEnableVertexAttribArray(pos_var.location);
    glEnableVertexAttribArray(texcoord_var.location);
    glEnableVertexAttribArray(normal_var.location);

    bind_policy.set_var(ShaderBindPolicy::kPosition, pos_var);
    bind_policy.set_var(ShaderBindPolicy::kTexcoord, texcoord_var);
    bind_policy.set_var(ShaderBindPolicy::kNormal, normal_var);

    const ShaderVariable &view_pos_var = uber_shader.prog().uniform_var("u_viewPosition");
    const ShaderVariable &mvp_var = uber_shader.prog().uniform_var("u_worldViewProjection");
    const ShaderVariable &world_var = uber_shader.prog().uniform_var("u_world");
    bind_policy.set_var(ShaderBindPolicy::kViewPosition, view_pos_var);
    bind_policy.set_var(ShaderBindPolicy::kWorldViewProjection, mvp_var);
    bind_policy.set_var(ShaderBindPolicy::kWorld, world_var);

    //set material 
    const ShaderVariable &ambient_var = uber_shader.prog().uniform_var("u_ambientColor");
    const ShaderVariable &diffuse_var = uber_shader.prog().uniform_var("u_diffuseColor");
    const ShaderVariable &specular_var = uber_shader.prog().uniform_var("u_specularColor");
    const ShaderVariable &shininess_var = uber_shader.prog().uniform_var("u_specularShininess");
    bind_policy.set_var(ShaderBindPolicy::kAmbientColor, ambient_var);
    bind_policy.set_var(ShaderBindPolicy::kDiffuseColor, diffuse_var);
    bind_policy.set_var(ShaderBindPolicy::kSpecularColor, specular_var);
    bind_policy.set_var(ShaderBindPolicy::kSpecularShininess, shininess_var);

    //apply bind policy
    render3d.SetShader(uber_shader.prog(), bind_policy);

    //set light position
    //GLint light_pos_handle = shader_prog.GetLocation(sora::kLocationWorldLightPosition);
    GLint light_pos_handle = uber_shader.prog().GetUniformLocation("u_worldLightPosition");
    glm::vec3 light_pos = glm::vec3(10, 10, 100);
    glUniform3fv(light_pos_handle, 1, glm::value_ptr(light_pos));
    GLHelper::CheckError("Set Light Pos Handle");
  
    //draw world
    ComponentList &mesh_comp_list = world.GetComponentList<MeshComponent>();
    auto it = mesh_comp_list.Begin();
    auto endit = mesh_comp_list.End();
    for(int i = 0; it != endit ; it++, i++) {
      //newmtl flatwhite
      //newmtl shinyred
      //newmtl clearblue
      if(i % 2 == 0) {
        Texture *tex = TextureMgr_get_ptr(string("sora"));
        Renderer::SetTexture(*tex);
        const sora::Material &mtl = MaterialMgr_get("clearblue");
        render3d.SetMaterial(mtl);
      } else {
        Texture *tex = TextureMgr_get_ptr(string("sora2"));
        Renderer::SetTexture(*tex);
        const sora::Material &mtl = MaterialMgr_get("shinyred");
        render3d.SetMaterial(mtl);
      }

      MeshComponent *comp = static_cast<MeshComponent*>(*it);
      switch(comp->mesh_type()) {
      case MeshComponent::kMeshObj:
        render3d.ApplyMatrix(comp->entity()->world_mat());
        render3d.DrawMesh(comp->obj_model());
        break;
      case MeshComponent::kMeshPrimitive:
        render3d.ApplyMatrix(comp->entity()->world_mat());
        render3d.DrawMesh(comp->primitive_model());
        break;
      default:
        break;
      }
    }
  }
  {
    Renderer &render2d = Renderer::Renderer2D();

    //draw 2d something
    glm::mat4 world_mat(1.0f);

    //shader 속성 설정
    ShaderBindPolicy bind_policy;
    //기본 속성
    const ShaderVariable &pos_var = shader_2d.attrib_var("a_position");
    const ShaderVariable &texcoord_var = shader_2d.attrib_var("a_texcoord");
    
    glEnableVertexAttribArray(pos_var.location);
    glEnableVertexAttribArray(texcoord_var.location);
    
    bind_policy.set_var(ShaderBindPolicy::kPosition, pos_var);
    bind_policy.set_var(ShaderBindPolicy::kTexcoord, texcoord_var);

    const ShaderVariable &mvp_var = shader_2d.uniform_var("u_worldViewProjection");
    bind_policy.set_var(ShaderBindPolicy::kWorldViewProjection, mvp_var);


    render2d.SetInitState();
    render2d.SetShader(shader_2d, bind_policy);
    render2d.ApplyMatrix(world_mat);
    sora::Font &font = sora::Font::GetInstance();
    render2d.SetTexture(font.font_texture());

    vector<sora::Vertex2D> vert_list;
    vert_list.push_back(sora::Vertex2D(100, 100, 0, 1));
    vert_list.push_back(sora::Vertex2D(100+128*2, 100, 1, 1));
    vert_list.push_back(sora::Vertex2D(100+128*2, 100+128*2, 1, 0));
    vert_list.push_back(sora::Vertex2D(100, 100+128*2, 0, 0));
    glVertexAttribPointer(pos_var.location, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &vert_list[0].pos);
    glVertexAttribPointer(texcoord_var.location, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &vert_list[0].texcoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    world_mat = glm::translate(world_mat, glm::vec3(0, 800, 0));
    world_mat = glm::scale(world_mat, glm::vec3(2, 2, 1));
    render2d.ApplyMatrix(world_mat);
    sora::Label label("PQRS_1234_asdf");
    glVertexAttribPointer(pos_var.location, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &label.vertex_data()->pos);
    glVertexAttribPointer(texcoord_var.location, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &label.vertex_data()->texcoord);
    glDrawElements(GL_TRIANGLES, label.index_count(), GL_UNSIGNED_SHORT, label.index_data());
  }
  

  //////////////////////////////
  Renderer::EndRender();
}

void SORA_setup_graphics(int w, int h) {
  setupGraphics(w, h);
}

void SORA_draw_frame() {
  renderFrame();
}

SR_C_DLL void SORA_init_gl_env() {
#if SR_WIN
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
}

TouchEventQueue touch_evt_queue;

void SORA_update_frame(float dt) {
  static float elapsed_time = 0;
  static int elapsed_tick_count = 0;
  elapsed_tick_count++;
  elapsed_time += dt;

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

  //터치 메세지 꺼내서 실제로 까보기
  while(touch_evt_queue.IsEmpty() == false) {
    TouchEvent evt = touch_evt_queue.Get();
    float x = (float)evt.x;
    float y = (float)evt.y;

    //x u x
    //l x r
    //x d x
    bool is_left = false;
    bool is_right = false;
    bool is_bottom = false;
    bool is_top = false;
    float scr_width = Renderer::win_width();
    float scr_height = Renderer::win_height();
    if(x < scr_width / 3) {
      is_left = true;
    }
    if(x > scr_width / 3.0f * 2) {
      is_right = true;
    }
    if(y < scr_height / 3.0f) {
      is_bottom = true;
    }
    if(y > scr_height / 3.0f * 2) {
      is_top = true;
    }

    float move_x = 5;
    if(is_top) {
      SORA_set_cam_pos(move_x, 0);
    }
    if(is_bottom) {
      SORA_set_cam_pos(-move_x, 0);
    }
    if(is_left) {
      SORA_set_cam_pos(0, move_x);
    }
    if(is_right) {
      SORA_set_cam_pos(0, -move_x);
    }


    //9버튼으로 조작할수 잇도록하자
    
    switch(evt.evt_type) {
    case kTouchBegan:
      LOGD("began [%d] %d,%d", evt.uid, evt.x, evt.y);
      break;
    case kTouchMoved:
      LOGD("moved [%d] %d,%d <- %d,%d", evt.uid, evt.x, evt.y, evt.prev_x, evt.prev_y);
      break;
    case kTouchEnded:
      LOGD("ended [%d] %d,%d", evt.uid, evt.x, evt.y);
      break;
    case kTouchCancelled:
      LOGD("canxx [%d] %d,%d", evt.uid, evt.x, evt.y);
      break;
    default:
      SR_ASSERT(!"do not reach");
      break;
    }
  }


#if SR_WIN
  float x = 0.1;
  //check key
  if(glfwGetKey('W') == GLFW_PRESS || glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
    SORA_set_cam_pos(x, 0);
  }
  if(glfwGetKey('S') == GLFW_PRESS || glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
    SORA_set_cam_pos(-x, 0);
  }
  if(glfwGetKey('A') == GLFW_PRESS || glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
    SORA_set_cam_pos(0, x);
  }
  if(glfwGetKey('D') == GLFW_PRESS || glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
    SORA_set_cam_pos(0, -x);
  }
#endif
}

void SORA_cleanup_graphics() {
}

SR_C_DLL void SORA_touch_began(int x, int y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchBegan;
  evt.prev_x = x;
  evt.prev_y = y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
  //LOGD("began");
}
SR_C_DLL void SORA_touch_moved(int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchMoved;
  evt.prev_x = prev_x;
  evt.prev_y = prev_y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
  //LOGD("moved");
}
SR_C_DLL void SORA_touch_ended(int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchEnded;
  evt.prev_x = prev_x;
  evt.prev_y = prev_y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
  //LOGD("ended");
}
SR_C_DLL void SORA_touch_cancelled(int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchCancelled;
  evt.prev_x = prev_x;
  evt.prev_y = prev_y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
}

#if SR_ANDROID
#include "sys/zip_stream_file.h"
void SORA_set_apk_file_path(const char *abs_path) {
  sora::ZipStreamFile::SetApkFile(abs_path);
}
#endif