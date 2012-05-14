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

#include "test_function.h"

//android  OpenGL ES 2.0 code based
#if SR_USE_PCH == 0
#include "gl_inc.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#endif

#include "device.h"
#include "memory_file.h"
#include "render_state.h"

#include "vertex.h"
#include "math_helper.h"

#include "renderer_env.h"
#include "shader.h"
#include "texture.h"
#include "buffer_object.h"
#include "image.h"
#include "renderer.h"
#include "frame_buffer.h"
#include "post_effect.h"
#include "mesh_buffer.h"

#include "geometric_object.h"
#include "freeglut_font.h"

#include "uber_shader.h"
#include "material.h"
#include "camera.h"
#include "light.h"

#include "obj_loader.h"
#include "obj_model.h"
#include "mesh.h"

#include "touch_device.h"
#include "touch_event.h"
#include "keyboard_event.h"

#include "gl_uber_shader_renderer.h"
#include "debug_draw_manager.h"
#include "fps_counter.h"
#include "draw_2d_manager.h"

using namespace std;
using namespace sora;
using namespace glm;

//테스트용 물체를 그릴수있도록 필요한 변수를 하드코딩으로 떄려박자
//const int kMaxObject = 10;
//vector<glm::mat4> world_mat_list(kMaxObject);
//vector<string> mesh_name_list(kMaxObject);

//빛1개를 전역변수처럼 쓰자
Light light;

Shader simple_shader;
PostEffect null_post_effect;

VertexBufferObject vbo;
IndexBufferObject wire_ibo;

FrameBuffer depth_fbo;

//uber shader직통으로 써보기
sora::gl::GLUberShaderRenderer uber_renderer;

FpsCounter fps_counter;


void SORA_set_window_size(Device *device, int w, int h) {
  device->render_state().SetWinSize(w, h);
}

bool setupGraphics(Device *device, int w, int h) {
  device->render_state().SetWinSize(w, h);
  depth_fbo.InitAsDepthTex(w, h);
  uber_renderer.Init();   //uber shader느낌으로 렌더링하기

  LOGI("Version : %s", RendererEnv::Version().c_str());
  LOGI("Vendor : %s", RendererEnv::Vender().c_str());
  LOGI("Renderer : %s", RendererEnv::Renderer().c_str());
  LOGI("Extensions List");
  auto ext_list = RendererEnv::ExtensionList();
  auto ext_it = ext_list.begin();
  auto ext_endit = ext_list.end();
  for( ; ext_it != ext_endit ; ++ext_it) {
    LOGI("%s", ext_it->c_str());
  }
  
  {
    //create shader
    string simple_vs_path = Filesystem::GetAppPath("shader/simple.vs");
    string simple_fs_path = Filesystem::GetAppPath("shader/simple.fs");
    simple_shader.LoadFromFile(simple_vs_path, simple_fs_path);
  }
  
  {
    //post effect
    string vs_path = Filesystem::GetAppPath("posteffect/shared.vs");
    string null_fs_path = Filesystem::GetAppPath("posteffect/null.fs");
    null_post_effect.InitFromFile(vs_path, null_fs_path);
  }
  //lodepng
  const char *texture_table[][2] = {
    { "sora", "texture/sora.png" },
    { "sora2", "texture/sora2.png" },
    { "mtl_diffuse", "texture/glazed_brick_D.png" },
    { "mtl_specular", "texture/glazed_brick_S.png" },
    { "mtl_normal", "texture/glazed_brick_N.png" },
    //{ "mtl_normal", "texture/normal_map.png" },
    
  };
  
  int tex_count = sizeof(texture_table) / sizeof(texture_table[0]);
  for(int i = 0 ; i < tex_count ; i++) {
    std::string tex_path = sora::Filesystem::GetAppPath(texture_table[i][1]);
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex(texture_table[i][0]);
    tex.Init();

    Image img;
    img.LoadPNG(tex_file.start, tex_file.end - tex_file.start);
    tex.LoadTexture(img);
    device->tex_mgr()->Add(tex);
  }
  {
    //load jpeg
    std::string tex_path = sora::Filesystem::GetAppPath("texture/img_cheryl.jpg");
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex("jellyfish");
    tex.Init();

    Image img;
    img.LoadJPG(tex_file.start, tex_file.end - tex_file.start);
    tex.LoadTexture(img);
    device->tex_mgr()->Add(tex);
  }
  
  {
    //load material
    sora::MtlLoader loader;
    std::string mtl_path = Filesystem::GetAppPath("material/example.mtl");
    MemoryFile mtl_file(mtl_path);
    mtl_file.Open();
    vector<sora::Material> material_list;
    loader.Load(mtl_file.start, mtl_file.end, &material_list);
    
    device->mtl_mgr()->Add(material_list);
  }
  {
    //load model
    std::string path1 = sora::Filesystem::GetAppPath("obj/cube.obj");
    //std::string path1 = sora::Filesystem::GetAppPath("obj/Beautiful Girl.obj");
    //std::string path1 = sora::Filesystem::GetAppPath("obj/beautiful_girl.obj");
    //std::string path1 = sora::Filesystem::GetAppPath("obj/Saber Fate Stay.obj");
    sora::MemoryFile file1(path1);
    file1.Open();
    ObjLoader loader;
    ObjModel obj_model;
    loader.Load(file1.start, file1.end, &obj_model);

    //Mesh *mesh = new Mesh();
    //mesh->Register(obj_model.cmd_list());
    //device->render_state().mesh_mgr().Add("mesh", mesh);
    
    /*
    //첫번쨰 물체 = obj model
    int obj_model_idx = 0;
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    //entity_mat = glm::rotate(glm::mat4(1.0f), 180.0f, vec3(1, 0, 0));
    //entity_mat = glm::scale(entity_mat, vec3(0.03, 0.03, 0.03));  //vanilla
    //entity_mat = glm::scale(entity_mat, vec3(0.1, 0.1, 0.1));  //fate, beautiful_girl
    world_mat_list[obj_model_idx] = entity_mat;

    //device->mesh_mgr().Add(obj_model.GetDrawCmdList_solid(), "obj_model");
    mesh_name_list[obj_model_idx] = "obj_model";
    */
  }
  {
    //primitive model test
    GeometricObject<Vertex> geo_obj;
    //GeometricObject<TangentVertex> geo_obj;
    //geo_obj.PointTeapot(0.05f);
    //geo_obj.WireTeapot(0.05f);
    //geo_obj.SolidTeapot(0.05f);
    //geo_obj.WireShpere(1, 16, 16);
    //geo_obj.PointShpere(1, 16, 16);
    //geo_obj.SolidSphere(1, 16, 16);
    //geo_obj.SolidCube(1, 1, 1);
    //geo_obj.WireCube(1, 1, 1);
    //geo_obj.PointCube(1, 1, 1);
    //geo_obj.PointCylinder(1, 1, 2, 8, 8);
    //geo_obj.WireCylinder(1, 1, 2, 8, 8);
    //geo_obj.SolidCylinder(1, 1, 2, 8, 8);
    //geo_obj.WireAxis(5);
    //geo_obj.SolidPlane(3);
    //geo_obj.WirePlane(3, 0.1f);
    geo_obj.SolidTorus(1.0f, 0.3f);
    //geo_obj.SolidCone(2, 2);

    Mesh *mesh = new Mesh();
    mesh->Register(geo_obj.cmd_list());
    device->mesh_mgr()->Add("mesh", mesh);
  }
  {
    //빛에 대한 기본 설정
    light.pos = vec3(0, 0, 100);
    //light.ambient = vec4(3.0f, 0, 0, 1.0f);
  }
  return true;
}

float aptitude = 10; //위도. -90~90. 세로 위치 표현
float latitude = 10; //경도

void SORA_set_cam_pos(float a, float b) {
  aptitude += a;
  latitude += b;
  if(aptitude > 90) {
    aptitude = 90;
  } else if(aptitude < -90) {
    aptitude = -90;
  }
}

void renderFrame(Device *device) {
  SR_CHECK_ERROR("Begin RenderFrame");
  Renderer::SetClearColor(0.5f, 0.0f, 0.0f, 1.0f);
  Renderer::ClearScreen();


  //3d
  device->render_state().Set3D();
  /*
  depth_fbo.Bind();  //fbo로 그리기. deferred같은거 구현하기 위해서 임시로 시도함
  device->render_state().Set3D();
  Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  Renderer::ClearScreen();
  */
  //일반 3d객체 그리기+카메라 회전 장착
  {
    //set material
    Material mtl;
    mtl.ambient = vec4(0.1, 0.1, 0.1, 1);
    mtl.diffuse = vec4(0.3, 0.3, 0.3, 1);
    //mtl.diffuse = vec4(1, 1, 1, 1);
    mtl.specular = vec4(1);
    mtl.shininess = 20;
    //mtl.diffuse_map = "mtl_diffuse";
    mtl.diffuse_map = "sora2";
    mtl.specular_map = "mtl_specular";
    mtl.normal_map = "mtl_normal";
    mtl.props |= kMaterialAmbient;
    mtl.props |= kMaterialDiffuse;
    mtl.props |= kMaterialDiffuseMap;
    mtl.props |= kMaterialSpecular;
    mtl.props |= kMaterialSpecularMap;
    //mtl.props |= kMaterialNormalMap;

    uber_renderer.SetMaterial(mtl);
    uber_renderer.SetLight(light);
    Shader &shader = uber_renderer.GetCurrShader();
    device->render_state().UseShader(shader);
    uber_renderer.ApplyMaterialLight();

    
    //device->render_state().UseShader(simple_shader);
   
    //set camera + projection
    //float win_width = (float)device->render_state().win_width();
    //float win_height = (float)device->render_state().win_height();
    //glm::mat4 projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);

    float radius = 4;
    float cam_x = radius * cos(SR_DEG_2_RAD(aptitude)) * sin(SR_DEG_2_RAD(latitude));
    float cam_y = radius * sin(SR_DEG_2_RAD(aptitude));
    float cam_z = radius * cos(SR_DEG_2_RAD(aptitude)) * cos(SR_DEG_2_RAD(latitude));
    Camera cam;
    cam.eye = vec3(cam_x, cam_y, cam_z);
    cam.center = vec3(0);
    cam.up = vec3(0, 1, 0);
    /*
    glm::mat4 view = cam.LookAt();

    glm::mat4 mvp(1.0f);
    mvp = projection * view;
    ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
    SR_ASSERT(mvp_var.location != -1);
    SetUniformMatrix(mvp_var, mvp);
    SR_CHECK_ERROR("SetMatrix");
    */
    uber_renderer.SetCamera(cam);

    Mesh *mesh = device->mesh_mgr()->Get("mesh");
    shader.DrawMeshIgnoreMaterial(mesh);
  }
  //depth_fbo.Unbind();

  //fbo에 있는 내용을 적절히 그리기
  //null_post_effect.Draw(depth_fbo.color_tex(), &device->render_state());

  
  {
    Draw2DManager *draw_2d_mgr = device->draw_2d();
    //디버깅용으로 화면 2d좌표계에 렌더링 하는거
    draw_2d_mgr->AddString(vec2(100, 100), "asd", Color_Red(), 2.0f);
    draw_2d_mgr->AddLine(vec2(100, 100), vec2(150, 200), Color_Blue(), 4.0f);
    draw_2d_mgr->AddSphere(vec2(200, 200), 30, Color_Green());
    draw_2d_mgr->AddCross(vec2(200, 200), Color_Green(), 5);
    
    //fps카운터 적절히 렌더링
    char fps_buf[16];
    sprintf(fps_buf, "FPS:%.2f", fps_counter.GetFPS());
    //float scr_width = device->render_state().win_width();
    float scr_height = (float)device->render_state().win_height();
    draw_2d_mgr->AddString(vec2(0, scr_height), fps_buf, Color_White(), 1.5f);

    sora::Draw2DPolicy draw_policy;
    draw_policy.Draw(*draw_2d_mgr);
  }
  {
    //디버깅용으로 화면 3d렌더링 하는거
    DebugDrawManager *mgr_3d = device->debug_draw_mgr();
    mgr_3d->AddAxis(mat4(1.0f), 10);
    mgr_3d->AddLine(vec3(0.0f, 0.0f, 0.0f), vec3(100, 100, 100), Color_White(), 4);
    mgr_3d->AddSphere(vec3(0, 0, 0), 3, Color_White());
    mgr_3d->AddString(vec3(5, 1, 1), "asdf", Color_Blue(), 2);


    DebugDrawPolicy debug_draw;
    debug_draw.Draw(*mgr_3d);
  }

  

  //////////////////////////////
  SR_CHECK_ERROR("End RenderFrame");
}

void SORA_setup_graphics(Device *device, int w, int h) {
  setupGraphics(device, w, h);
}

void SORA_draw_frame(Device *device) {
  renderFrame(device);
}

void SORA_init_gl_env() {
#if SR_WIN
#endif
}

void SORA_update_frame(Device *device, float dt) {
  fps_counter.EndFrame(dt);
  Draw2DManager *draw_2d_mgr = device->draw_2d();
  draw_2d_mgr->Update(dt);
  DebugDrawManager *mgr_3d = device->debug_draw_mgr();
  mgr_3d->Update(dt);


  TouchEventQueue &touch_evt_queue = device->touch_evt_queue();

#if SR_WIN && (SR_GLES == 0)
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
  int elapsed_tick_count = fps_counter.total_frame_count();
  float elapsed_second = fps_counter.elapsed_second();
  touch_device.UpdateState(left_btn_state, posx, posy, elapsed_tick_count, elapsed_second);
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
    float scr_width = (float)device->render_state().win_width();
    float scr_height = (float)device->render_state().win_height();
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
      //LOGD("began [%d] %d,%d", evt.uid, evt.x, evt.y);
      break;
    case kTouchMoved:
      //LOGD("moved [%d] %d,%d <- %d,%d", evt.uid, evt.x, evt.y, evt.prev_x, evt.prev_y);
      break;
    case kTouchEnded:
      //LOGD("ended [%d] %d,%d", evt.uid, evt.x, evt.y);
      break;
    case kTouchCancelled:
      //LOGD("canxx [%d] %d,%d", evt.uid, evt.x, evt.y);
      break;
    default:
      SR_ASSERT(!"do not reach");
      break;
    }
  }

  //check key
  float x = 1.0f;
  KeyboardEventQueue &keyboard_evt_queue = device->keyboard_evt_queue();
  while(keyboard_evt_queue.IsEmpty() == false) {
    KeyboardEvent evt = keyboard_evt_queue.Get();
    if(evt.is_special_key) {
      switch(evt.ch) {
      case KeyboardEvent::kUp:
        SORA_set_cam_pos(x, 0);
        break;
      case KeyboardEvent::kDown:
        SORA_set_cam_pos(-x, 0);
        break;
      case KeyboardEvent::kLeft:
        SORA_set_cam_pos(0, x);
        break;
      case KeyboardEvent::kRight:
        SORA_set_cam_pos(0, -x);
        break;
      }
    }
  }
}

void SORA_cleanup_graphics(Device *device) {
}

SR_C_DLL void SORA_touch_began(Device *device, int x, int y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchBegan;
  evt.prev_x = x;
  evt.prev_y = y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  TouchEventQueue &touch_evt_queue = device->touch_evt_queue();
  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
  //LOGD("began");
}
SR_C_DLL void SORA_touch_moved(Device *device, int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchMoved;
  evt.prev_x = prev_x;
  evt.prev_y = prev_y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  TouchEventQueue &touch_evt_queue = device->touch_evt_queue();
  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
  //LOGD("moved");
}
SR_C_DLL void SORA_touch_ended(Device *device, int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchEnded;
  evt.prev_x = prev_x;
  evt.prev_y = prev_y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  TouchEventQueue &touch_evt_queue = device->touch_evt_queue();
  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
  //LOGD("ended");
}
SR_C_DLL void SORA_touch_cancelled(Device *device, int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp) {
  TouchEvent evt;
  evt.evt_type = kTouchCancelled;
  evt.prev_x = prev_x;
  evt.prev_y = prev_y;
  evt.x = x;
  evt.y = y;
  evt.tick_count = tick_count;
  evt.timestamp = timestamp;
  evt.uid = uid;

  TouchEventQueue &touch_evt_queue = device->touch_evt_queue();
  touch_evt_queue.Push(evt);
  SR_ASSERT(touch_evt_queue.IsEmpty() == false);
}

#if SR_ANDROID
#include "zip_stream_file.h"
void SORA_set_apk_file_path(const char *abs_path) {
  sora::ZipStreamFile::SetApkFile(abs_path);
}
#endif