﻿/*  Copyright (C) 2011-2012 by if1live */
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
#if SR_ANDROID
#include <jni.h>
#endif

#if SR_USE_PCH == 0
#include "gl_inc.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#endif

#include "vector.h"
#include "gl_helper.h"
#include "memory_file.h"

#include "shader.h"
#include "matrix_stack.h"

#include "obj_model.h"
#include "obj_loader.h"
#include "primitive_model.h"
#include "material_manager.h"

#include "texture.h"
#include "renderer.h"
#include "material.h"
#include "math_helper.h"
#include "camera.h"
#include "font.h"
#include "entity.h"
#include "world.h"
#include "mesh_component.h"
#include "component_list.h"

using namespace std;
using namespace sora;
using namespace glm;

//fixed
//sora::Texture tex(sora::Texture::kPolicyForcePOT);
sora::Texture tex;

ShaderProgram shader_prog;
ShaderProgram shader_2d;

//cbes
World world;

bool setupGraphics(int w, int h) {
  sora::Renderer::GetInstance().SetWindowSize((float)w, (float)h);
 
  //load shader file
  {
    std::string app_vert_path = sora::Filesystem::GetAppPath("shader/v_diffuse.glsl");
    std::string app_frag_path = sora::Filesystem::GetAppPath("shader/f_diffuse.glsl");
    sora::MemoryFile vert_file(app_vert_path);
    sora::MemoryFile frag_file(app_frag_path);
    vert_file.Open();
    frag_file.Open();
    const char *vert_src = (const char*)(vert_file.start);
    const char *frag_src = (const char*)(frag_file.start);
    bool prog_result = shader_prog.Init(vert_src, frag_src);
    if(prog_result == false) {
      LOGE("Could not create program.");
      return false;
    }
  }

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

  /*
  //create sample texture
  GLubyte pixel_data[4*3] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255,
    255, 255, 0,
  };
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLuint tex_id = -1;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  tex.Init(tex_id, 2, 2);
  */
  //lodepng
  std::string tex_path = sora::Filesystem::GetAppPath("texture/sora.png");
  sora::MemoryFile tex_file(tex_path);
  tex_file.Open();
  tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
  tex.Init();

  sora::ObjLoader loader;

  //load model
  std::string path1 = sora::Filesystem::GetAppPath("obj/cube.obj");
  sora::MemoryFile file1(path1);
  file1.Open();
  ObjModel obj_model;
  loader.LoadObj(file1.start, file1.end, &obj_model);

  //load material
  MaterialManager::GetInstance().LoadFromFile();

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
    entity_mat = glm::translate(entity_mat, vec3(0.5, 1, 0));
    entity_mat = glm::rotate(entity_mat, 180.0f, vec3(1, 0, 0));
    entity_a->set_world_mat(entity_mat);
    
    MeshComponent *mesh_comp = new MeshComponent(entity_a);
    primitive_model.SolidCube(0.5, 1, 0.5, true);
    mesh_comp->SetMesh(primitive_model);
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
  sora::Renderer &renderer = sora::Renderer::GetInstance();

  {
    //render 3d
    renderer.Set3D();

    //use texture
    renderer.SetShader(shader_prog);
    renderer.SetTexture(tex);

    //use material
    //sora::Material &mtl = material_list[0];
    //newmtl flatwhite
    //newmtl shinyred
    //newmtl clearblue
    //const sora::Material &mtl = sora::MaterialManager::GetInstance().Get("sample");
    const sora::Material &mtl = sora::MaterialManager::GetInstance().Get("sample");
    renderer.SetMaterial(mtl);
  
    int position_handle = shader_prog.GetLocation(sora::ShaderVariable::kPosition);
    int texcoord_handle = shader_prog.GetLocation(sora::ShaderVariable::kTexcoord);
    int normal_handle = shader_prog.GetLocation(sora::ShaderVariable::kNormal);
  
    glEnableVertexAttribArray(position_handle);
    glEnableVertexAttribArray(texcoord_handle);
    glEnableVertexAttribArray(normal_handle);

    //set camera + projection
    float win_width = renderer.win_width();
    float win_height = renderer.win_height();
    glm::mat4 &projection = renderer.projection_mat();
    projection = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);
    float radius = 4;
    float cam_x = radius * cos(SR_DEG_2_RAD(aptitude)) * sin(SR_DEG_2_RAD(latitude));
    float cam_y = radius * sin(SR_DEG_2_RAD(aptitude));
    float cam_z = radius * cos(SR_DEG_2_RAD(aptitude)) * cos(SR_DEG_2_RAD(latitude));

    sora::Camera cam;
    cam.set_eye(sora::Vec3f(cam_x, cam_y, cam_z));
    cam.set_dir(sora::Vec3f(0, 0, 0) - cam.eye());
    cam.set_up(sora::Vec3f(0, 1, 0));
    renderer.set_camera(cam);

    //set light position
    //GLint light_pos_handle = shader_prog.GetLocation(sora::kLocationWorldLightPosition);
    GLint light_pos_handle = shader_prog.GetUniformLocation("u_worldLightPosition");
    glm::vec3 light_pos = glm::vec3(10, 10, 100);
    glUniform3fv(light_pos_handle, 1, glm::value_ptr(light_pos));
    GLHelper::CheckError("Set Light Pos Handle");
  
    //draw world
    ComponentList &mesh_comp_list = world.GetComponentList<MeshComponent>();
    auto it = mesh_comp_list.Begin();
    auto endit = mesh_comp_list.End();
    for( ; it != endit ; it++) {
      MeshComponent *comp = static_cast<MeshComponent*>(*it);
      comp->Draw(&renderer);
    }
  }
  {
    //draw 2d something
    glm::mat4 world_mat(1.0f);

    renderer.Set2D();
    renderer.SetShader(shader_2d);
    renderer.ApplyMatrix2D(world_mat);
    sora::Font &font = sora::Font::GetInstance();
    renderer.SetTexture(font.font_texture());

    int position_handle = shader_2d.GetLocation(sora::ShaderVariable::kPosition);
    int texcoord_handle = shader_2d.GetLocation(sora::ShaderVariable::kTexcoord);

    vector<sora::Vertex2D> vert_list;
    vert_list.push_back(sora::Vertex2D(100, 100, 0, 1));
    vert_list.push_back(sora::Vertex2D(100+128*2, 100, 1, 1));
    vert_list.push_back(sora::Vertex2D(100+128*2, 100+128*2, 1, 0));
    vert_list.push_back(sora::Vertex2D(100, 100+128*2, 0, 0));
    glVertexAttribPointer(position_handle, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &vert_list[0].pos);
    glVertexAttribPointer(texcoord_handle, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &vert_list[0].texcoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    world_mat = glm::translate(world_mat, glm::vec3(0, 800, 0));
    world_mat = glm::scale(world_mat, glm::vec3(2, 2, 1));
    renderer.ApplyMatrix2D(world_mat);
    sora::Label label("PQRS_1234_asdf");
    glVertexAttribPointer(position_handle, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &label.vertex_data()->pos);
    glVertexAttribPointer(texcoord_handle, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex2D), &label.vertex_data()->texcoord);
    glDrawElements(GL_TRIANGLES, label.index_count(), GL_UNSIGNED_SHORT, label.index_data());

  }
  

  //////////////////////////////
  renderer.EndRender();
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

void SORA_update_frame(float dt) {
  
}

void SORA_cleanup_graphics() {
}

#if SR_ANDROID
#include "zip_stream_file.h"
void SORA_set_apk_file_path(const char *abs_path) {
  sora::ZipStreamFile::SetApkFile(abs_path);
}
#endif