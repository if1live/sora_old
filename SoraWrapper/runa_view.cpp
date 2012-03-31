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

using namespace std;
using namespace glm;

namespace sora {;

//테스트용 물체를 그릴수있도록 필요한 변수를 하드코딩으로 떄려박자
const int kMaxObject = 10;
vector<glm::mat4> world_mat_list(kMaxObject);
vector<string> mesh_name_list(kMaxObject);


struct RunaViewPrivate {
  RunaViewPrivate()
    : uber_flag(ShaderFlag::kNone) {
      const_color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
      mtl.diffuse_map = "mtl_diffuse";
      mtl.specular_map = "mtl_specular";

      mtl.illumination_model = 2;
      mtl.diffuse[0] = 0.1f;
      mtl.diffuse[1] = 0.1f;
      mtl.diffuse[2] = 0.1f;
      mtl.ambient[0] = 0.01f;
      mtl.ambient[1] = 0.01f;
      mtl.ambient[2] = 0.01f;
      mtl.shininess = 50;
  }
  Material mtl;
  Light light;
  Vec4f const_color;
  ShaderFlag uber_flag;
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

void RunaView::SetShaderFlag(bool b, ShaderFlag value)
{
  if(b) {
    EnableShaderFlag(value);
  } else {
    DisableShaderFlag(value);
  }
}

RunaView::RunaView() : pimpl_(NULL) {
}
RunaView::~RunaView() {
  if(pimpl_ != NULL) {
    delete(pimpl_);
    pimpl_ = NULL;
  }
}
void RunaView::SetupGraphics(int w, int h) {
  device().render_state().SetWinSize(w, h);

  LOGI("Version : %s", GLHelper::GetVersion().c_str());
  LOGI("Vendor : %s", GLHelper::GetVender().c_str());
  LOGI("Renderer : %s", GLHelper::GetRenderer().c_str());
  LOGI("Extensions : %s", GLHelper::GetExtensions().c_str());
  
  //lodepng
  const char *texture_table[][2] = {
    //{ "sora", "texture/sora.png" },
    { "sora2", "texture/sora2.png" },
    { "mtl_diffuse", "texture/glazed_brick_D.png" },
    { "mtl_specular", "texture/glazed_brick_S.png" },
    //{ "mtl_normal", "texture/glazed_brick_N.png" },
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

  sora::ObjLoader loader;
  //load material
  {
    std::string mtl_path = Filesystem::GetAppPath("material/example.mtl");
    MemoryFile mtl_file(mtl_path);
    mtl_file.Open();
    vector<sora::Material> material_list;
    ObjLoader loader;
    loader.LoadMtl(mtl_file.start, mtl_file.end, &material_list);
    
    device().material_mgr().Add(material_list);
  }

  {
    //load model
    std::string path1 = sora::Filesystem::GetAppPath("obj/cube.obj");
    sora::MemoryFile file1(path1);
    file1.Open();
    ObjModel obj_model;
    loader.LoadObj(file1.start, file1.end, &obj_model);

    //첫번쨰 물체 = obj model
    int obj_model_idx = 0;
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    entity_mat = glm::rotate(glm::mat4(1.0f), 180.0f, vec3(1, 0, 0));
    world_mat_list[obj_model_idx] = entity_mat;

    device().mesh_mgr().Add(obj_model.GetDrawCmdList_wire(), "obj_model");
    mesh_name_list[obj_model_idx] = "obj_model";
  }

  {
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

    //두번쨰 물체 = 썡물체
    int obj_model_idx = 1;
    glm::mat4 entity_mat = glm::mat4(1.0f);
    //-1로 하면 그리기가 영향을 받아서 망(vert가 뒤집히면서 그리기 방향도 뒤집혀 버림)
    //entity_mat = glm::scale(glm::mat4(1.0f), vec3(1, -1, 1)); 
    //entity_mat = glm::rotate(glm::mat4(1.0f), 180.0f, vec3(1, 0, 0));
    world_mat_list[obj_model_idx] = entity_mat;

    //primitive_model.SolidSphere(0.5, 16, 16);
    //primitive_model.WirePlane(10.0f, 0.5f);
    primitive_model.SolidPlane(2.0f);
    device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), "model1");
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
    
    sora::PrimitiveModel primitive_model;
    //primitive_model.SolidCube(0.5, 0.5, 0.5, true);
    primitive_model.SolidSphere(0.5, 16, 16);
    device().mesh_mgr().Add(primitive_model.GetDrawCmdList(), "model2");
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

    TrefoilKnot surface(1.0f);
    device().mesh_mgr().AddSolid(surface, "knot");
    //MeshManager::GetInstance().AddWire(surface, "knot");
    mesh_name_list[obj_model_idx] = "knot";
  }

  {
    //빛에 대한 기본 설정
    pimpl().light.pos = Vec3f(10, 10, 100);
    //light.ambient = Vec4f(3.0f, 0, 0, 1.0f);
  }
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
void RunaView::SetWindowSize(int w, int h) {
  device().render_state().SetWinSize(w, h);
}
void RunaView::DrawFrame() {
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  {
    //uber shader
    Renderer &render3d = device().render3d();
    render3d.SetInitState();
    
    //set camera + projection
    float win_width = (float)device().render_state().win_width();
    float win_height = (float)device().render_state().win_height();
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

    unsigned int flag = (unsigned int)pimpl().uber_flag;
    //unsigned int flag = 0;
    //flag |= UberShader::kConstColor;
    //flag |= UberShader::kTexture;
    //flag |= UberShader::kAmbientColor;
    //flag |= UberShader::kDiffuseColor;
    //flag |= UberShader::kDiffuseMap;
    //flag |= UberShader::kSpecularColor;
    //flag |= UberShader::kSpecularMap;
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
    int obj_idx = 2;
    const mat4 &world_mat = world_mat_list[obj_idx];
    render3d.ApplyMatrix(world_mat);

    //재질데이터 적절히 설정하기
    render3d.SetMaterial(pimpl().mtl);
    /*
    Material mtl;
    mtl.diffuse_map = "mtl_diffuse";
    mtl.specular_map = "mtl_specular";
    mtl.illumination_model = 2;
    mtl.diffuse[0] = 0.1f;
    mtl.diffuse[1] = 0.1f;
    mtl.diffuse[2] = 0.1f;
    mtl.ambient[0] = 0.01f;
    mtl.ambient[1] = 0.01f;
    mtl.ambient[2] = 0.01f;
    mtl.shininess = 50;
    render3d.SetMaterial(mtl);
    */
    render3d.ApplyMaterialLight();
      
    MeshBufferObject *mesh = device().mesh_mgr().Get(mesh_name_list[obj_idx]);
    //SR_ASSERT(mesh != NULL);
    render3d.Draw(*mesh);
    GLHelper::CheckError("Render End");
  }
  
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