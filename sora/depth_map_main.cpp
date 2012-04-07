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
#include "depth_map_main.h"

#include "sys/filesystem.h"
#include "renderer/shader.h"
#include "sys/memory_file.h"
#include "renderer/texture.h"
#include "renderer/texture_manager.h"
#include "renderer/gl_helper.h"

#include "renderer/light.h"
#include "renderer/mesh_manager.h"
#include "sys/device.h"
#include "mesh/primitive_model.h"
#include "renderer/gl_buffer_object.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

using namespace std;
using namespace glm;

namespace sora {;
namespace depthmap {
  //표준 렌더링을 하는데
  //깊이버퍼 텍스쳐로 적절히 획득하자

  float win_width = 0;
  float win_height = 0;

  ShaderProgram depth_tex_shader;
  ShaderProgram gray_depth_tex_shader;
  //ShaderProgram *curr_depth_shader = &gray_depth_tex_shader;
  ShaderProgram *curr_depth_shader = &depth_tex_shader;

  const char *kCube1 = "cube1";
  const char *kPlane = "plane";

  //깊이를 텍스쳐에 연결
  GLuint fbo = 0;
  GLuint depth_tex = 0;
  GLuint color_rb = 0;

  void setup_graphics(sora::Device *dev, int w, int h) {
    win_width = (float)w;
    win_height = (float)h;

    //create shader
    {
      std::string app_vert_path = sora::Filesystem::GetAppPath("shader/depth_tex.vs");
      std::string app_frag_path = sora::Filesystem::GetAppPath("shader/depth_tex.fs");
      depth_tex_shader.LoadFromFile(app_vert_path, app_frag_path);
    }
    {
      std::string app_vert_path = sora::Filesystem::GetAppPath("shader/gray_depth_tex.vs");
      std::string app_frag_path = sora::Filesystem::GetAppPath("shader/gray_depth_tex.fs");
      gray_depth_tex_shader.LoadFromFile(app_vert_path, app_frag_path);
    }
    {
      //테스트용 평면
      /*
      sora::PrimitiveModel primitive_model;
      primitive_model.SolidPlane(5);
      dev->mesh_mgr().Add(primitive_model.GetDrawCmdList(), kPlane);
      */
    }
    {
      /*
      //테스트용 큐브
      sora::PrimitiveModel primitive_model;
      primitive_model.SolidCube(2, 2, 4);
      dev->mesh_mgr().Add(primitive_model.GetDrawCmdList(), kCube1);
      */
    }
    {
      glGenFramebuffers(1, &fbo);
      //색은 그냥 버퍼로 쓰자
      glGenRenderbuffers(1, &color_rb);
      glBindRenderbuffer(GL_RENDERBUFFER, color_rb);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, w, h);
      GLHelper::CheckError("1");

      //깊이를 텍스쳐에 연결
      glGenTextures(1, &depth_tex);
      glBindTexture(GL_TEXTURE_2D, depth_tex);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
      GLHelper::CheckError("DepthTexture");

      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);
      GLHelper::CheckFrameBufferStatus("fb");
      GLHelper::CheckError("Create FB");
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    {
      std::string tex_path = sora::Filesystem::GetAppPath("texture/sora.png");
      sora::MemoryFile tex_file(tex_path);
      tex_file.Open();
      Texture tex("sora");
      tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
      dev->texture_mgr().Add(tex);
    }
    {
      //기본 설정
      glEnable(GL_DEPTH_TEST);  //depthmap이 목적이니 깊이 테스트 해야지
    }

    GLHelper::CheckError("DepthMap Init");
  }
  void draw_frame(sora::Device *dev) {
    {
      //fbo에 렌더링
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, (int)win_width, (int)win_height);

      Renderer &render3d = dev->render3d();
      render3d.SetShader(dev->simple_shader());

      Camera cam;
      cam.eye = vec3(-4, 6, 10);
      cam.center = vec3(0);
      cam.up = vec3(0, 1, 0);
      render3d.set_camera(cam);

      float projection_near = 0.1f;
      float projection_far = 30.0f;
      glm::mat4 projection = glm::perspective(45.0f, win_width / win_height, projection_near, projection_far);
      render3d.set_projection_mat(projection);

      glm::mat4 model(1.0f);
      render3d.ApplyMatrix(model);

      Texture *tex = dev->texture_mgr().Get_ptr(string("sora"));
      render3d.SetTexture(*tex);

      vector<string> mesh_list;
      mesh_list.push_back(kCube1);  //0 : cube
      mesh_list.push_back(kPlane);  //1 : plane
      for(size_t i = 0 ; i < mesh_list.size() ; i++) {
        const char *name = mesh_list[i].c_str();
        MeshBufferObject *mesh_buffer = dev->mesh_mgr().Get(name);
        render3d.Draw(*mesh_buffer);
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    GLHelper::CheckError("FBO draw");
    {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, (int)win_width, (int)win_height);

      //깊이맵을 적절히 렌더링
      float vertex[] = {
        -1, -1,
        1, -1,
        1, 1,
        -1, 1
      };
      float texcoord[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1,
      };
      
      glUseProgram(curr_depth_shader->prog);

      int pos_loc = curr_depth_shader->GetAttribLocation("a_position");
      int tex_loc = curr_depth_shader->GetAttribLocation("a_texcoord");
      glEnableVertexAttribArray(pos_loc);
      glEnableVertexAttribArray(tex_loc);
      int mvp_loc = curr_depth_shader->GetUniformLocation("u_worldViewProjection");
      glm::mat4 mvp(1.0f);
      glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

      //Texture *tex = tex_mgr.Get_ptr(string("sora"));
      //glBindTexture(GL_TEXTURE_2D, tex->handle());
      glBindTexture(GL_TEXTURE_2D, depth_tex);
      //glBindTexture(GL_TEXTURE_2D, color_tex);

      glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 0, vertex);
      glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }
    GLHelper::CheckError("Draw DepthMap");
    Renderer::EndRender();
  }

  void update_frame(sora::Device *dev, float dt) {
#if SR_WIN && (SR_GLES == 0)
    //깊이맵용 쉐이더 교체
    if(glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
      //알아보기 쉽게 확 튀는 색으로 뜨는거. 대신 시각적으로 깊이 알아보기 어려움
      curr_depth_shader = &depth_tex_shader;
    }
    if(glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
      //그레이 스케일. *100배가 존재함
      curr_depth_shader = &gray_depth_tex_shader;
    }
#endif
  }
}
}
