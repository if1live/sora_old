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
#include "Stdafx.h"

#include "runa_view.h"

#include "sys/device.h"
#include "renderer/render_state.h"
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
#include "renderer/texture_manager.h"

#include "renderer/shader_bind_policy.h"

#include "event/touch_device.h"
#include "event/touch_event.h"

#include "renderer/parametric_equations.h"
#include "renderer/parametric_surface.h"
#include "renderer/mesh_manager.h"
#include "renderer/light.h"

#include "core/math_helper.h"


using namespace std;
using namespace glm;

#include <msclr/marshal_cppstd.h>


using namespace System;
//using namespace System::Configuration;

namespace sora {;

const char *kTextureKey = "texture";
const char *kDiffuseMapKey = "diffuse";
const char *kSpecularMapKey = "specular";

struct RunaViewPrivate {
  RunaViewPrivate()
    : uber_flag(ShaderFlag::kNone),
    light_move(false),
    light_pos_deg(0),
    camDeg(0) {
      const_color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
      mtl.diffuse_map = kDiffuseMapKey;
      mtl.specular_map = kSpecularMapKey;

      //uber flag를 적절히 줘야 정상적인 쉐이더 겟지만 
      //이거는 에디터같은 느낌으로 만들엇기 떄문에 무시하고 쓸수 있도록해놨다

      mtl.diffuse[0] = 0.1f;
      mtl.diffuse[1] = 0.1f;
      mtl.diffuse[2] = 0.1f;
      mtl.ambient[0] = 0.01f;
      mtl.ambient[1] = 0.01f;
      mtl.ambient[2] = 0.01f;
      mtl.shininess = 50;

      model_name = "sphere";
  }
  Material mtl;
  Light light;
  Vec4f const_color;
  ShaderFlag uber_flag;
  bool light_move;
  float light_pos_deg;

  float camDeg;
  string model_name;

  //나중에 세이브 로드 위해서 남겨놓자
  std::string simple_tex_path;
  std::string diffuse_map_path;
  std::string specular_map_path;
};


void RunaView::SetAmbientColor(Byte r, Byte g, Byte b) {
  RunaViewPrivate &impl = pimpl();
  impl.mtl.ambient[0] = r / 255.0f;
  impl.mtl.ambient[1] = g / 255.0f;
  impl.mtl.ambient[2] = b / 255.0f;
}
void RunaView::SetDiffuseColor(Byte r, Byte g, Byte b) {
  RunaViewPrivate &impl = pimpl();
  impl.mtl.diffuse[0] = r / 255.0f;
  impl.mtl.diffuse[1] = g / 255.0f;
  impl.mtl.diffuse[2] = b / 255.0f;
}
void RunaView::SetSpecularColor(Byte r, Byte g, Byte b) {
  RunaViewPrivate &impl = pimpl();
  impl.mtl.specular[0] = r / 255.0f;
  impl.mtl.specular[1] = g / 255.0f;
  impl.mtl.specular[2] = b / 255.0f;
}
void RunaView::SetConstColor(Byte r, Byte g, Byte b) {
  RunaViewPrivate &impl = pimpl();
  impl.const_color = Vec4f(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}
void RunaView::GetConstColor(array<Byte> ^%color) {
  RunaViewPrivate &impl = pimpl();
  color[0] = (Byte)(impl.const_color.x * 255);
  color[1] = (Byte)(impl.const_color.y * 255);
  color[2] = (Byte)(impl.const_color.z * 255);
}

void RunaView::GetAmbientColor(array<Byte> ^%color) {
  RunaViewPrivate &impl = pimpl();
  for(int i = 0 ; i < 3 ; i++) {
    color[i] = (Byte)(impl.mtl.ambient[i] * 255);
  }

}

void RunaView::GetDiffuseColor(array<Byte> ^%color) {
  RunaViewPrivate &impl = pimpl();
  for(int i = 0 ; i < 3 ; i++) {
    color[i] = (Byte)(impl.mtl.diffuse[i] * 255);
  }
}
void RunaView::GetSpecularColor(array<Byte> ^%color) {
  RunaViewPrivate &impl = pimpl();
  for(int i = 0 ; i < 3 ; i++) {
    color[i] = (Byte)(impl.mtl.specular[i] * 255);
  }
}

float RunaView::GetSpecularShininess() {
  return pimpl().mtl.shininess;
}

void RunaView::SetSpecularShininess(float shininess) {
  pimpl().mtl.shininess = shininess;
}

ShaderFlag RunaView::GetShaderFlag() {
  return pimpl().uber_flag;
}
void RunaView::EnableShaderFlag(ShaderFlag value) {
  ShaderFlag curr_flag = GetShaderFlag();
  ShaderFlag flag = curr_flag | value;
  pimpl().uber_flag = flag;
}
void RunaView::DisableShaderFlag(ShaderFlag value) {
  ShaderFlag curr_flag = GetShaderFlag();
  ShaderFlag flag = curr_flag & (~value);
  pimpl().uber_flag = flag;
}
bool RunaView::IsEnabledShaderFlag(ShaderFlag value) {
  ShaderFlag curr_flag = GetShaderFlag();
  if((curr_flag & value) == value) {
    return true;
  } else {
    return false;
  }
}

void RunaView::SetShaderFlag(bool b, ShaderFlag value) {
  if(b) {
    EnableShaderFlag(value);
  } else {
    DisableShaderFlag(value);
  }
}

void RunaView::SetLightMove(bool b) {
  pimpl().light_move = b;
}
void RunaView::SetLightAmbientColor(Byte r, Byte g, Byte b) {
  pimpl().light.ambient[0] = r / 255.0f;
  pimpl().light.ambient[1] = g / 255.0f;
  pimpl().light.ambient[2] = b / 255.0f;
}
void RunaView::SetLightDiffuseColor(Byte r, Byte g, Byte b) {
  pimpl().light.diffuse[0] = r / 255.0f;
  pimpl().light.diffuse[1] = g / 255.0f;
  pimpl().light.diffuse[2] = b / 255.0f;
}
void RunaView::SetLightSpecularColor(Byte r, Byte g, Byte b)  {
  pimpl().light.specular[0] = r / 255.0f;
  pimpl().light.specular[1] = g / 255.0f;
  pimpl().light.specular[2] = b / 255.0f;
}
  
bool RunaView::IsLightMove() {
  return pimpl().light_move;
}
void RunaView::GetLightAmbientColor(array<Byte> ^%color) {
  for(int i = 0 ; i < 3 ; i++) {
    color[i] = (Byte)(pimpl().light.ambient[0] * 255);
  }
}
void RunaView::GetLightDiffuseColor(array<Byte> ^%color) {
  for(int i = 0 ; i < 3 ; i++) {
    color[i] = (Byte)(pimpl().light.diffuse[0] * 255);
  }
}
void RunaView::GetLightSpecularColor(array<Byte> ^%color) {
  for(int i = 0 ; i < 3 ; i++) {
    color[i] = (Byte)(pimpl().light.specular[0] * 255);
  }
}

void RunaView::SetTexturePath(String ^tex_path, String ^ext) {
  //.ext로 확장자는 넘어온다
  String ^png_ext = gcnew String(".png");
  String ^jpg_ext = gcnew String(".jpg"); 
  string path_str = msclr::interop::marshal_as<std::string>(tex_path);
  string ext_str = msclr::interop::marshal_as<std::string>(ext);
  ReloadTexture(path_str, ext_str, kTextureKey);
}
void RunaView::SetDiffuseMapPath(System::String ^tex_path, System::String ^ext) {
  String ^png_ext = gcnew String(".png");
  String ^jpg_ext = gcnew String(".jpg"); 
  string path_str = msclr::interop::marshal_as<std::string>(tex_path);
  string ext_str = msclr::interop::marshal_as<std::string>(ext);
  ReloadTexture(path_str, ext_str, kDiffuseMapKey);
}
void RunaView::SetSpecularMapPath(System::String ^tex_path, System::String ^ext) {
  String ^png_ext = gcnew String(".png");
  String ^jpg_ext = gcnew String(".jpg"); 
  string path_str = msclr::interop::marshal_as<std::string>(tex_path);
  string ext_str = msclr::interop::marshal_as<std::string>(ext);
  ReloadTexture(path_str, ext_str, kSpecularMapKey);
}

void RunaView::SetModel(System::String ^name) {
  string model_name = msclr::interop::marshal_as<std::string>(name);
  pimpl().model_name = model_name;
}
void RunaView::ReloadTexture(const std::string &path, const std::string &ext, const char *key) {
  sora::MemoryFile tex_file(path);
  tex_file.Open();
  Texture tex(path);
  if(ext == ".png") {
    tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);  
  } else if(ext == ".jpg") {
    tex.SetData(sora::Texture::kFileJPEG, tex_file.start, tex_file.end);
  }

  //바꿔치기
  //데이터 바꿔치기식의 접근은 가능한가?
  Texture *orig_tex = device().texture_mgr().Get_ptr(string(key));
  orig_tex->Reload(tex);

  tex_file.Close();
}
///////////////////////////////////////////

RunaView::RunaView() 
: pimpl_(NULL) {
}
RunaView::~RunaView() {
  if(pimpl_ != NULL) {
    delete(pimpl_);
    pimpl_ = NULL;
  }
}

//테스트용 물체를 그릴수있도록 필요한 변수를 하드코딩으로 떄려박자
//wire plane + wire axis
vector<glm::mat4> world_mat_list(2);
vector<string> mesh_name_list(2);
vector<Material> mtl_list(2);

//테스트용으로 그려지는 모델에 대한 정보. 일단 대충 만들기
vector<glm::mat4> obj_world_mat_list;
vector<string> obj_mesh_name_list;

void RunaView::SetupGraphics(int w, int h) {
  device().render_state().SetWinSize(w, h);

  LOGI("Version : %s", GLHelper::GetVersion().c_str());
  LOGI("Vendor : %s", GLHelper::GetVender().c_str());
  LOGI("Renderer : %s", GLHelper::GetRenderer().c_str());
  //LOGI("Extensions : %s", GLHelper::GetExtensions().c_str());
  
  //lodepng
  //그냥, diffuse specular용으로 사용할것을 미리 만들어놓자
  const char *texture_table[][2] = {
    { kTextureKey, "texture/sora2.png" },
    { kSpecularMapKey, "texture/glazed_brick_S.png" },
    { kDiffuseMapKey, "texture/glazed_brick_D.png" },
  };
  int tex_count = sizeof(texture_table) / sizeof(texture_table[0]);
  for(int i = 0 ; i < tex_count ; i++) {
    std::string tex_path = sora::Filesystem::GetAppPath(texture_table[i][1]);
    sora::MemoryFile tex_file(tex_path);
    tex_file.Open();
    Texture tex(texture_table[i][0]);
    tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
    device().texture_mgr().Add(tex);
  }
  
  //0번물체는 wire 평면
  {
    const int idx = 0;
    world_mat_list[idx] = glm::mat4(1.0f);

    Material &mtl = mtl_list[idx];
    mtl.uber_flag = UberShader::kConstColor;

    sora::PrimitiveModel primitive_model;
    primitive_model.WirePlane(5, 0.2);
    std::string mesh_name = "wire[plane";
    device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), mesh_name.c_str());
    mesh_name_list[idx] = mesh_name;
  }
  //1번은 wire axis
  {
    const int idx = 1;
    world_mat_list[idx] = glm::mat4(1.0f);

    Material &mtl = mtl_list[idx];
    mtl.uber_flag = UberShader::kModelColor;

    sora::PrimitiveModel primitive_model;
    primitive_model.WireAxis(5);
    std::string mesh_name = "wire_axis";
    device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), mesh_name.c_str());
    mesh_name_list[idx] = mesh_name;
  }

  {
    //그려질 물체
    glm::mat4 reverse_mat = glm::mat4(1.0f);  //대부분 180뒤집기가 필요할거다
    reverse_mat = glm::rotate(reverse_mat, 180.0f, vec3(1, 0, 0));

    {
      //0 : sphere
      obj_world_mat_list.push_back(reverse_mat);
      PrimitiveModel primitive_model;
      primitive_model.SolidSphere(0.5, 16, 16);
      const char *name = "sphere";
      device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), name);
      obj_mesh_name_list.push_back(string(name));
    }
    {
      //1 : cube
      obj_world_mat_list.push_back(reverse_mat);
      PrimitiveModel primitive_model;
      primitive_model.SolidCube(1, 1, 1, true);
      const char *name = "cube";
      device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), name);
      obj_mesh_name_list.push_back(string(name));
      
    }
    {
      //2 teapot
      mat4 entity_mat = glm::rotate(reverse_mat, 90.0f, vec3(1, 0, 0));
      obj_world_mat_list.push_back(entity_mat);
      
      PrimitiveModel primitive_model;
      primitive_model.SolidTeapot(2);
      const char *name = "teapot";
      device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), name);
      obj_mesh_name_list.push_back(string(name));
    }
    {
      obj_world_mat_list.push_back(glm::mat4(1.0f));
      TrefoilKnot surface(1.0f);
      const char *name = "TrefoilKnot";
      device().mesh_mgr().AddSolid(surface, name);
      obj_mesh_name_list.push_back(string(name));
    }
    {
      obj_world_mat_list.push_back(glm::mat4(1.0f));
      Torus surface(0.5f, 0.2f);
      const char *name = "Torus";
      device().mesh_mgr().AddSolid(surface, name);
      obj_mesh_name_list.push_back(string(name));
    }
    {
      obj_world_mat_list.push_back(glm::mat4(1.0f));
      KleinBottle surface(0.14f);
      const char *name = "KleinBottle";
      device().mesh_mgr().AddSolid(surface, name);
      obj_mesh_name_list.push_back(string(name));
    }
    {
      obj_world_mat_list.push_back(glm::mat4(1.0f));
      MobiusStrip surface(0.7);
      const char *name = "MobiusStrip";
      device().mesh_mgr().AddSolid(surface, name);
      obj_mesh_name_list.push_back(string(name));
    }
  }
}

float aptitude = 10; //위도. -90~90. 세로 위치 표현
//float latitude = 10; //경도

void RunaView::SetWindowSize(int w, int h) {
  device().render_state().SetWinSize(w, h);
}

void RunaView::DrawFrame() {
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //uber shader
  Renderer &render3d = device().render3d();
  render3d.SetInitState();
    
  //set camera + projection
  float win_width = (float)device().render_state().win_width();
  float win_height = (float)device().render_state().win_height();
  glm::mat4 &projection = render3d.projection_mat();
  projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);
  float radius = 4;
  float latitude = pimpl().camDeg;
  float cam_x = radius * cos(SR_DEG_2_RAD(aptitude)) * sin(SR_DEG_2_RAD(latitude));
  float cam_y = radius * sin(SR_DEG_2_RAD(aptitude));
  float cam_z = radius * cos(SR_DEG_2_RAD(aptitude)) * cos(SR_DEG_2_RAD(latitude));

  sora::Camera cam;
  cam.set_eye(sora::Vec3f(cam_x, cam_y, cam_z));
  cam.set_dir(sora::Vec3f(0, 0, 0) - cam.eye());
  cam.set_up(sora::Vec3f(0, 1, 0));
  render3d.set_camera(cam);

  //plane, axis같은 디버깅 관련 요소 그리기
  //선을 겹쳐 그릴수 잇으니까 깊이끈다
  for(size_t i = 0 ; i < world_mat_list.size() ; i++) {
    unsigned int flag = mtl_list[i].uber_flag;
    ShaderProgram &shader = device().uber_shader(flag);
    render3d.SetShader(shader);

    ShaderBindPolicy &bind_policy = shader.bind_policy;
    const ShaderVariable &const_color_var = bind_policy.var(ShaderBindPolicy::kConstColor);
    if(const_color_var.location != -1) {
      Vec4f const_color(1.0f, 1.0f, 1.0f, 1.0f);
      glUniform4fv(const_color_var.location, 1, const_color.data);
    }

    //색만 잇는걸로 디버깅할테니 다른 속성은 빼자
    const glm::mat4 &world_mat = world_mat_list[i];
    render3d.ApplyMatrix(world_mat);
  
    MeshBufferObject *mesh = device().mesh_mgr().Get(mesh_name_list[i]);
    //SR_ASSERT(mesh != NULL);

    if(i == 1) {
      glDisable(GL_DEPTH_TEST);
      render3d.Draw(*mesh);
      glEnable(GL_DEPTH_TEST);
    } else {
      render3d.Draw(*mesh);
    } 
  }

  {
    //원하는 이름의 모델 얻기
    int model_idx = -1;
    for(size_t i = 0 ; i < obj_mesh_name_list.size() ; i++) {
      if(obj_mesh_name_list[i] == pimpl().model_name) {
        model_idx = i;
        break;
      }
    }
    if(model_idx >= 0) {
      glm::mat4 &world_mat = obj_world_mat_list[model_idx];
      const string &mesh_name = obj_mesh_name_list[model_idx];

      //모델 적절히 그리기
      unsigned int flag = (unsigned int)pimpl().uber_flag;
      ShaderProgram &shader = device().uber_shader(flag);
      render3d.SetShader(shader);
      ShaderBindPolicy &bind_policy = shader.bind_policy;

      const ShaderVariable &const_color_var = bind_policy.var(ShaderBindPolicy::kConstColor);
      if(const_color_var.location != -1) {
        Vec4f &const_color = pimpl().const_color;
        glUniform4fv(const_color_var.location, 1, const_color.data);
      }

      //평면하나만 일단 렌더링해서 테스트하자
      render3d.SetLight(pimpl().light);
      render3d.ApplyMatrix(world_mat);

      //텍스쳐 적당히 설정
      Texture *tex = device().texture_mgr().Get_ptr(string(kTextureKey));
      if(tex != NULL) {
        render3d.SetTexture(*tex);
      }

      //재질데이터 적절히 설정하기
      render3d.SetMaterial(pimpl().mtl);
      render3d.ApplyMaterialLight();
      
      MeshBufferObject *mesh = device().mesh_mgr().Get(mesh_name);
      //SR_ASSERT(mesh != NULL);
      render3d.Draw(*mesh);
    }
  }
  GLHelper::CheckError("Render End");
}
void RunaView::InitGLEnv() {
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void RunaView::UpdateFrame(float dt) {
  GLHelper::CheckError("Update Begin");

  float prev_light_deg = pimpl().light_pos_deg;
  if(pimpl().light_move == true) {
    //적절한 속도로 빛 움직이기
    prev_light_deg += 100 * dt;
  }
  //빛 위치 적절히 재설정
  float light_pos_radius = 100;
  float light_pos_x = sin(DegToRad(prev_light_deg)) * light_pos_radius;
  float light_pos_z = cos(DegToRad(prev_light_deg)) * light_pos_radius;
  pimpl().light.pos = Vec3f(light_pos_x, 10, light_pos_z);

  //apply
  pimpl().light_pos_deg = prev_light_deg;

  //카메라 적절히 설정
}

void RunaView::SetCamRotateDeg(float deg) {
  pimpl().camDeg = deg;
}
void RunaView::Cleanup() {
}

RunaViewPrivate &RunaView::pimpl() {
  if(pimpl_ == NULL) {
    pimpl_ = new RunaViewPrivate();
  }
  return *pimpl_;
}
}