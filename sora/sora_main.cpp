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
#include "renderer/gl_inc.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#endif

#include "sys/device.h"
#include "sys/memory_file.h"

#include "core/vertex.h"
#include "core/math_helper.h"

#include "renderer/renderer_env.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/buffer_object.h"

#include "mesh/geometric_object.h"

//#include "renderer/uber_shader.h"
//#include "renderer/matrix_stack.h"

//#include "renderer/obj_model.h"
//#include "renderer/obj_loader.h"
//#include "mesh/primitive_model_builder.h"
//#include "renderer/material_manager.h"

//#include "renderer/texture.h"
//#include "renderer/renderer.h"

//#include "renderer/material.h"
//#include "renderer/camera.h"
//#include "renderer/font.h"
//#include "renderer/texture_manager.h"

//#include "renderer/shader_bind_policy.h"

#include "event/touch_device.h"
#include "event/touch_event.h"
#include "event/keyboard_event.h"

//#include "mesh/parametric_equations.h"
//#include "mesh/parametric_surface.h"
//#include "renderer/mesh_manager.h"
//#include "renderer/light.h"



using namespace std;
using namespace sora;
using namespace glm;

//테스트용 물체를 그릴수있도록 필요한 변수를 하드코딩으로 떄려박자
//const int kMaxObject = 10;
//vector<glm::mat4> world_mat_list(kMaxObject);
//vector<string> mesh_name_list(kMaxObject);

//빛1개를 전역변수처럼 쓰자
//Light light;

Shader simple_shader;
Shader color_shader;

VertexBufferObject vbo;
IndexBufferObject wire_ibo;

void SORA_set_window_size(Device *device, int w, int h) {
  device->render_device().SetWinSize(w, h);
}

bool setupGraphics(Device *device, int w, int h) {
  device->render_device().SetWinSize(w, h);

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

    string color_vs_path = Filesystem::GetAppPath("shader/const_color.vs");
    string color_fs_path = Filesystem::GetAppPath("shader/const_color.fs");
    color_shader.LoadFromFile(color_vs_path, color_fs_path);
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
    tex.SetData(sora::kTexFilePNG, tex_file.start, tex_file.end);
    device->render_device().tex_mgr().Add(tex);
  }
  {
    //load jpeg
    std::string tex_path = sora::Filesystem::GetAppPath("texture/img_cheryl.jpg");
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex("jellyfish");
    tex.SetData(sora::kTexFileJPEG, tex_file.start, tex_file.end);
    device->render_device().tex_mgr().Add(tex);
  }
  
  /*
  sora::ObjLoader loader;
  //load material
  {
    std::string mtl_path = Filesystem::GetAppPath("material/example.mtl");
    MemoryFile mtl_file(mtl_path);
    mtl_file.Open();
    vector<sora::Material> material_list;
    ObjLoader loader;
    loader.LoadMtl(mtl_file.start, mtl_file.end, &material_list);
    
    device->material_mgr().Add(material_list);
  }
  */
  /*
  {
    //load model
    std::string path1 = sora::Filesystem::GetAppPath("obj/cube.obj");
    //std::string path1 = sora::Filesystem::GetAppPath("obj/Beautiful Girl.obj");
    //std::string path1 = sora::Filesystem::GetAppPath("obj/beautiful_girl.obj");
    //std::string path1 = sora::Filesystem::GetAppPath("obj/Saber Fate Stay.obj");
    sora::MemoryFile file1(path1);
    file1.Open();
    ObjModel obj_model;
    loader.LoadObj(file1.start, file1.end, &obj_model);

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
  }
  */
  /*
  {
    //primitive model test
    //sora::PrimitiveModel primitive_model;
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

    //두번쨰 물체 = 썡물체
    int obj_model_idx = 1;
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    //entity_mat = glm::rotate(glm::mat4(1.0f), 180.0f, vec3(1, 0, 0));
    world_mat_list[obj_model_idx] = entity_mat;

    //primitive_model.SolidSphere(0.5, 16, 16);
    //primitive_model.WirePlane(10.0f, 0.5f);
    //primitive_model.SolidPlane(2.0f);
    //primitive_model.WireAxis(3);
    //primitive_model.SolidCylinder(0.5, 2, 16);
    //primitive_model.SolidCone(1, 2, 8, 8);
    //device->mesh_mgr().Add(primitive_model.GetDrawCmdList(), "model1");
    mesh_name_list[obj_model_idx] = "model1";
  }

  {
    //세번쨰 물체 = 썡물체
    int obj_model_idx = 2;
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    //entity_mat = glm::translate(entity_mat, vec3(0.8, 0.3, 0));
    //entity_mat = glm::rotate(entity_mat, 180.0f, vec3(1, 0, 0));
    world_mat_list[obj_model_idx] = entity_mat;
    
    //sora::PrimitiveModel primitive_model;
    //primitive_model.SolidCube(0.5, 0.5, 0.5, true);
    //primitive_model.SolidSphere(0.5, 16, 16);
    //device->mesh_mgr().Add(primitive_model.GetDrawCmdList(), "model2");
    vector<float> vert_data;
    IndexListType index_list;

    unsigned int total_flag = 0;
    total_flag |= PrimitiveModelBuilder::kFlagColor;
    total_flag |= PrimitiveModelBuilder::kFlagTexcoord;
    total_flag |= PrimitiveModelBuilder::kFlagNormal;

    MeshBufferObject mesh_obj;
    //mesh_obj = PrimitiveModelBuilder::WireCube<TangentVertex>(0, 1, 1, 1);
    //mesh_obj = PrimitiveModelBuilder::WireSphere<TangentVertex>(0, 0.5, 16, 16);
    //mesh_obj = PrimitiveModelBuilder::SolidSphere<TangentVertex>(total_flag, 0.5, 16, 16);
    //mesh_obj = PrimitiveModelBuilder::WireAxis<TangentVertex>(PrimitiveModelBuilder::kFlagColor, 1);
    //mesh_obj = PrimitiveModelBuilder::WirePlane<TangentVertex>(0, 5, 0.2);
    //mesh_obj = PrimitiveModelBuilder::SolidPlane<TangentVertex>(PrimitiveModelBuilder::kFlagTexcoord, 5);
    //mesh_obj = PrimitiveModelBuilder::WireTeapot<TangentVertex>(0, 2);
    //mesh_obj = PrimitiveModelBuilder::SolidTeapot<TangentVertex>(0, 2);
    //mesh_obj = PrimitiveModelBuilder::WireCone<TangentVertex>(0, 1, 2, 8, 8);
    mesh_obj = PrimitiveModelBuilder::WireCylinder<TangentVertex>(0, 1, 2, 8);

    device->mesh_mgr().Add(mesh_obj, "model2");
    mesh_name_list[obj_model_idx] = "model2";

  }

  {
    //surfaces[0] = new Cone(2, 1);
    //surfaces[1] = new Sphere(0.5f);
    //surfaces[2] = new Torus(0.5f, 0.2f);
    //surfaces[3] = new TrefoilKnot(1.0f);
    //surfaces[4] = new KleinBottle(0.2f);
    //surfaces[5] = new MobiusStrip(1);
    
    //네번째 물체
    int obj_model_idx = 3;

    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    entity_mat = glm::translate(entity_mat, vec3(0, 1, 0));
    world_mat_list[obj_model_idx] = entity_mat;

    TrefoilKnot surface(1.5f);
    //Sphere surface(1.0);
    //KleinBottle surface(0.2f);

    //MeshBufferObject mesh_obj = surface.CreateSolidMeshObject<TangentVertex>();
    MeshBufferObject mesh_obj = surface.CreateSolidMeshObject<TangentVertex>();
    device->mesh_mgr().Add(mesh_obj, "knot");
    //MeshManager::GetInstance().AddWire(surface, "knot");
    mesh_name_list[obj_model_idx] = "knot";
  }

  {
    //빛에 대한 기본 설정
    light.pos = vec3(10, 10, 100);
    //light.ambient = vec4(3.0f, 0, 0, 1.0f);
  }
  */
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
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SR_CHECK_ERROR("Begin RenderFrame");

  //3d
  device->render_device().Set3D();

  VertexList vert_list;
  vert_list.push_back(Vertex(vec3(-0.5, -0.5, 0), vec2(0, 0)));
  vert_list.push_back(Vertex(vec3(0.5, -0.5, 0), vec2(1, 0)));
  vert_list.push_back(Vertex(vec3(0, 0.5, 0), vec2(0.5, 1)));
  
  //vbo, ibo 적절히 만들기
  if(vbo.Loaded() == false) {
    vbo.Init(vert_list);
  }
  if(wire_ibo.Loaded() == false) {
    IndexList index_list;
    index_list.push_back(0);
    index_list.push_back(1);
    index_list.push_back(1);
    index_list.push_back(2);
    index_list.push_back(2);
    index_list.push_back(0);
    wire_ibo.Init(index_list);
  }

  {
    //shader사용 선언이 가장 먼저
    device->render_device().UseShader(color_shader);
    SR_CHECK_ERROR("UseShader");

    mat4 mvp(1.0f);
    color_shader.SetMatrix(kMVPHandleName, mvp);
    SR_CHECK_ERROR("SetMatrix");
    vec4 color(1.0f);
    color_shader.SetVector(kConstColorHandleName, color);
    SR_CHECK_ERROR("SetVector");

    //color_shader.SetVertexList(vert_list);
    //color_shader.DrawArrays(kDrawTriangles, vert_list.size());
    //color_shader.DrawArrays(kDrawTriangles, vbo);
    color_shader.DrawElements(kDrawLines, vbo, wire_ibo);
  }
  
  {
    //shader사용 선언이 가장 먼저
    device->render_device().UseShader(simple_shader);
    TexturePtr tex = device->render_device().tex_mgr().Get("sora");
    device->render_device().UseTexture(*tex);
    SR_CHECK_ERROR("UseShader");
    mat4 mvp(1.0f);
    mvp = glm::rotate(mvp, 10.0f, vec3(0, 0, 1));
    simple_shader.SetMatrix(kMVPHandleName, mvp);
    SR_CHECK_ERROR("SetMatrix");
    simple_shader.SetVertexList(vert_list);
    simple_shader.DrawArrays(kDrawTriangles, vert_list.size());
  }

  //일반 3d객체 그리기+카메라 회전 장착
  {
    //set camera + projection
    float win_width = (float)device->render_device().win_width();
    float win_height = (float)device->render_device().win_height();
    glm::mat4 projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);
    float radius = 4;
    float cam_x = radius * cos(SR_DEG_2_RAD(aptitude)) * sin(SR_DEG_2_RAD(latitude));
    float cam_y = radius * sin(SR_DEG_2_RAD(aptitude));
    float cam_z = radius * cos(SR_DEG_2_RAD(aptitude)) * cos(SR_DEG_2_RAD(latitude));
    vec3 eye(cam_x, cam_y, cam_z);
    vec3 center(0);
    vec3 up(0, 1, 0);
    glm::mat4 view = glm::lookAt(eye, center, up);

    glm::mat4 mvp(1.0f);
    mvp = projection * view;
    simple_shader.SetMatrix(kMVPHandleName, mvp);
    SR_CHECK_ERROR("SetMatrix");

    GeometricObject mesh;
    //mesh.PointTeapot(0.05f);
    //mesh.WireTeapot(0.05f);
    //mesh.SolidTeapot(0.05f);
    //mesh.WireShpere(1, 16, 16);
    //mesh.PointShpere(1, 16, 16);
    mesh.SolidSphere(1, 16, 16);
    auto it = mesh.Begin();
    auto endit = mesh.End();
    for( ; it != endit ; ++it) {
      const DrawCmdData &cmd = *it;
      simple_shader.SetVertexList(cmd.vertex_list);
      if(cmd.index_list.empty()) {
        simple_shader.DrawArrays(cmd.draw_mode, cmd.vertex_list.size());
      } else {
        simple_shader.DrawElements(cmd.draw_mode, cmd.index_list);
      }
    }
  }

  //draw 2d
  {
    SR_CHECK_ERROR("Render 2d start");
    device->render_device().Set2D();

    device->render_device().UseShader(simple_shader);
    sora::SysFont &font = device->render_device().sys_font();
    device->render_device().UseTexture(font.font_texture());

    //해상도에 맞춰서 적절히 설정
    float win_width = (float)device->render_device().win_width();
    float win_height = (float)device->render_device().win_height();
    glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
    simple_shader.SetMatrix(kMVPHandleName, projection);

    Vertex2DList vert_list;
    vert_list.push_back(sora::Vertex2D(100, 100, 0, 1));
    vert_list.push_back(sora::Vertex2D(100+128*2, 100, 1, 1));
    vert_list.push_back(sora::Vertex2D(100+128*2, 100+128*2, 1, 0));
    vert_list.push_back(sora::Vertex2D(100, 100+128*2, 0, 0));
    simple_shader.SetVertexList(vert_list);
    simple_shader.DrawArrays(kDrawTriangleFan, vert_list.size());

    mat4 world_mat(1.0f);
    world_mat = glm::translate(world_mat, glm::vec3(0, 800, 0));
    world_mat = glm::scale(world_mat, glm::vec3(2, 2, 1));
    mat4 mvp = projection * world_mat;
    simple_shader.SetMatrix(kMVPHandleName, mvp);
    sora::Label label(&font, "PQRS_1234_asdf");
    simple_shader.SetVertexList(label.vertex_list());
    simple_shader.DrawElements(kDrawTriangles, label.index_list());
  }

  /*
  {
    //uber shader
    Renderer &render3d = device->render3d();
    render3d.SetInitState();
    
    //set camera + projection
    float win_width = (float)device->render_state().win_width();
    float win_height = (float)device->render_state().win_height();
    glm::mat4 &projection = render3d.projection_mat();
    projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);
    float radius = 4;
    float cam_x = radius * cos(SR_DEG_2_RAD(aptitude)) * sin(SR_DEG_2_RAD(latitude));
    float cam_y = radius * sin(SR_DEG_2_RAD(aptitude));
    float cam_z = radius * cos(SR_DEG_2_RAD(aptitude)) * cos(SR_DEG_2_RAD(latitude));

    sora::Camera cam;
    cam.eye = vec3(cam_x, cam_y, cam_z);
    cam.center = vec3(0);
    cam.up = vec3(0, 1, 0);
    render3d.set_camera(cam);

    unsigned int flag = 0;
    flag |= UberShader::kAmbientColor;
    flag |= UberShader::kAmbientMap;
    flag |= UberShader::kDiffuseColor;
    flag |= UberShader::kDiffuseMap;
    flag |= UberShader::kSpecularColor;
    flag |= UberShader::kSpecularMap;
    flag |= UberShader::kNormalMap;
    ShaderProgram &shader = device->uber_shader(flag);
    render3d.SetShader(shader);

    ShaderBindPolicy &bind_policy = shader.bind_policy;

    //평면하나만 일단 렌더링해서 테스트하자
    render3d.SetLight(light);
    //int obj_idx = 2;
    //int obj_idx = 1;
    int obj_idx = 3;
    const mat4 &world_mat = world_mat_list[obj_idx];
    render3d.ApplyMatrix(world_mat);

    //Texture *tex = device->texture_mgr().Get_ptr(string("sora2"));
    //render3d.SetTexture(*tex);

    //재질데이터 적절히 설정하기
    Material mtl;
    mtl.ambient_map = "sora";
    mtl.diffuse_map = "mtl_diffuse";
    mtl.specular_map = "mtl_specular";
    mtl.normal_map = "mtl_normal";
    mtl.ambient = vec3(0.1, 0.1, 0.1);
    //mtl.ambient = vec3(1, 1, 1);
    mtl.diffuse = vec3(0.5, 0.5, 0.5);
    mtl.specular = vec3(0.5, 0.5, 0.5);
    mtl.shininess = 20;
    mtl.uber_flag = flag;
    render3d.SetMaterial(mtl);
    render3d.ApplyMaterialLight();
      
    MeshBufferObject *mesh = device->mesh_mgr().Get(mesh_name_list[obj_idx]);
    SR_ASSERT(mesh != NULL);
    render3d.Draw(*mesh);

    SR_CHECK_ERROR("Render End");
  }
  */

  //////////////////////////////
  
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
  static float elapsed_time = 0;
  static int elapsed_tick_count = 0;
  elapsed_tick_count++;
  elapsed_time += dt;

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
    float scr_width = (float)device->render_device().win_width();
    float scr_height = (float)device->render_device().win_height();
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
  float x = 3.0f;
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
#include "sys/zip_stream_file.h"
void SORA_set_apk_file_path(const char *abs_path) {
  sora::ZipStreamFile::SetApkFile(abs_path);
}
#endif