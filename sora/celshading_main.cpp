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
#include "celshading_main.h"

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
#include "core/math_helper.h"

#include "renderer/parametric_surface.h"
#include "renderer/parametric_equations.h"
#include "renderer/renderer.h"
#include "renderer/uber_shader.h"
#include "renderer/light.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

using namespace std;
using namespace glm;

namespace sora {;
namespace celshading {

  TextureManager tex_mgr;
  ShaderProgram edge_shader;

  float win_width = 0;
  float win_height = 0;
  float rot_deg = 0;
  Light light;

  //테스트용 모델 이름
  const char *kSolid = "cube1";
  const char *kWire = "cube2";

  float cel_aptitude = 10; //위도. -90~90. 세로 위치 표현
  float cel_latitude = 10; //경도

  void set_cam_pos(float a, float b) {
    cel_aptitude += a;
    cel_latitude += b;
    if(cel_aptitude > 90) {
      cel_aptitude = 90;
    } else if(cel_aptitude < -90) {
      cel_aptitude = -90;
    }
  }

  void setup_graphics(sora::Device *dev, int w, int h) {
    win_width = w;
    win_height = h;

    //edge 그리기
    {
      std::string app_vert_path = sora::Filesystem::GetAppPath("shader/edge.vs");
      std::string app_frag_path = sora::Filesystem::GetAppPath("shader/edge.fs");
      edge_shader.LoadFromFile(app_vert_path, app_frag_path);
    }
    {
      //쉐도우 테스트용 큐브
      //sora::PrimitiveModel primitive_model;
      //primitive_model.SolidCube(2, 2, 2, true);
      //primitive_model.SolidCube(2, 2, 2, true);
      //dev->mesh_mgr().Add(primitive_model.GetDrawCmdList(), kCube1);
      //Torus surface(0.5f, 0.2f);
      TrefoilKnot surface(1.0f);

      dev->mesh_mgr().AddSolid(surface, kSolid);
      dev->mesh_mgr().AddWire(surface, kWire);
    }

    {
      //set light
      light.pos = vec3(10, 10, 100);
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


  void draw_frame(sora::Device *dev) {
    //공통 속성
    float radius = 5;
    float cam_x = radius * cos(SR_DEG_2_RAD(cel_aptitude)) * sin(SR_DEG_2_RAD(cel_latitude));
    float cam_y = radius * sin(SR_DEG_2_RAD(cel_aptitude));
    float cam_z = radius * cos(SR_DEG_2_RAD(cel_aptitude)) * cos(SR_DEG_2_RAD(cel_latitude));

    sora::Camera cam;
    cam.eye = vec3(cam_x, cam_y, cam_z);
    cam.center = vec3(0, 0, 0);
    cam.up = vec3(0, 1, 0);

    mat4 projection_mat = glm::perspective(45.0f, win_width / win_height, 0.1f, 100.0f);

    //obj rendering
    glClearColor(0.3f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, win_width, win_height);
    

    //edge 그리기
    {
      //깊이테스트 조절해서 뒤에 선그리게 하자
      //glDepthFunc(GL_GEQUAL);

      float edge_width = 3.0f;
      glLineWidth(edge_width);
      glUseProgram(edge_shader.prog);
      int pos_loc = edge_shader.GetAttribLocation("a_position");
      int normal_loc = edge_shader.GetAttribLocation("a_normal");
      glEnableVertexAttribArray(pos_loc);
      glEnableVertexAttribArray(normal_loc);
      int mvp_loc = edge_shader.GetUniformLocation("u_worldViewProjection");

      //set camera + projection
      glm::mat4 view = glm::lookAt(cam.eye, cam.center, cam.up);
      glm::mat4 model(1.0f);
      glm::mat4 mvp = projection_mat * view * model;
      glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

      //edge색 적절히 조정
      int color_loc = edge_shader.GetUniformLocation("u_constColor");
      float color[4] = { 0.1, 0.1, 0.1, 1.0 };
      glUniform4fv(color_loc, 1, color);

      //폴리곤 오프셋으로 외곽선적절히 처리
      //폴리곤 오프셋 = thickness x dist x fovx / width 
      //http://www.gamedevforever.com/18
      float poly_offset = edge_width * radius * 45.0f / win_width;
      glEnable(GL_POLYGON_OFFSET_FILL);
      //glPolygonOffset(poly_offset, 1.0);
      glPolygonOffset(4.0, 2.0);

      vector<string> mesh_list;
      mesh_list.push_back(kWire);
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
        glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, 0);
      }
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //모델 그리기
    {
      glClear(GL_DEPTH_BUFFER_BIT);
      //glDepthFunc(GL_LEQUAL);
      //uber shader
      Renderer &render3d = dev->render3d();
      render3d.SetInitState();

      //set camera + projection
      glm::mat4 &projection = render3d.projection_mat();
      projection = projection_mat;
      render3d.set_camera(cam);

      unsigned int flag = 0;
      flag |= UberShader::kAmbientColor;
      //flag |= UberShader::kAmbientMap;
      flag |= UberShader::kDiffuseColor;
      //flag |= UberShader::kDiffuseMap;
      flag |= UberShader::kSpecularColor;
      //flag |= UberShader::kSpecularMap;
      ShaderProgram &shader = dev->uber_shader(flag);
      render3d.SetShader(shader);

      ShaderBindPolicy &bind_policy = shader.bind_policy;

      //평면하나만 일단 렌더링해서 테스트하자
      render3d.SetLight(light);
      const mat4 world_mat(1.0f);
      render3d.ApplyMatrix(world_mat);

      //Texture *tex = device->texture_mgr().Get_ptr(string("sora2"));
      //render3d.SetTexture(*tex);

      //재질데이터 적절히 설정하기
      Material mtl;
      mtl.ambient_map = "sora2";
      mtl.diffuse_map = "mtl_diffuse";
      mtl.specular_map = "mtl_specular";
      mtl.ambient = vec3(0.3, 0.3, 0.3);
      mtl.diffuse = vec3(0.5, 0.5, 0.5);
      mtl.specular = vec3(0.5, 0.5, 0.0);
      mtl.shininess = 50;
      mtl.uber_flag = flag;
      render3d.SetMaterial(mtl);
      render3d.ApplyMaterialLight();

      MeshBufferObject *mesh = dev->mesh_mgr().Get(kSolid);
      SR_ASSERT(mesh != NULL);
      render3d.Draw(*mesh);
    }

    GLHelper::CheckError("glDrawArrays");
    Renderer::EndRender();
  }

  void update_frame(sora::Device *dev, float dt) {
#if SR_WIN && (SR_GLES == 0)
    float x = 0.1f;
    //check key
    if(glfwGetKey('W') == GLFW_PRESS || glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
      set_cam_pos(x, 0);
    }
    if(glfwGetKey('S') == GLFW_PRESS || glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
      set_cam_pos(-x, 0);
    }
    if(glfwGetKey('A') == GLFW_PRESS || glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
      set_cam_pos(0, x);
    }
    if(glfwGetKey('D') == GLFW_PRESS || glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
      set_cam_pos(0, -x);
    }
#endif

  }
}
}