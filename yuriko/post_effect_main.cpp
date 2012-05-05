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
#include "post_effect_main.h"

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

#include "geometric_object.h"

using namespace std;
using namespace sora;
using namespace glm;

namespace sora {;
namespace posteffect {
  Shader simple_shader;
  Shader color_shader;
  PostEffect null_post_effect;
  PostEffect grayscale_post_effect;

  VertexBufferObject vbo;
  IndexBufferObject wire_ibo;

  FrameBuffer depth_fbo;

  void setup_graphics(Device *device, int w, int h) {
    device->render_device().SetWinSize(w, h);
    depth_fbo.InitAsDepthTex(w, h);

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
    {
      //post effect
      string vs_path = Filesystem::GetAppPath("posteffect/shared.vs");
      string to_grayscale_fs_path = Filesystem::GetAppPath("posteffect/to_grayscale.fs");
      string null_fs_path = Filesystem::GetAppPath("posteffect/null.fs");
      grayscale_post_effect.InitFromFile(vs_path, to_grayscale_fs_path);
      null_post_effect.InitFromFile(vs_path, null_fs_path);
    }
    //lodepng
    const char *texture_table[][2] = {
      { "sora", "texture/sora.png" },
      { "sora2", "texture/sora2.png" }, 
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
      device->render_device().tex_mgr().Add(tex);
    }
  }

  float aptitude = 10; //위도. -90~90. 세로 위치 표현
  float latitude = 10; //경도

  void draw_frame(Device *device) {
    SR_CHECK_ERROR("Begin RenderFrame");
    Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    Renderer::ClearScreen();

    //3d
    device->render_device().Set3D();

    depth_fbo.Bind();  //fbo로 그리기. deferred같은거 구현하기 위해서 임시로 시도함
    device->render_device().Set3D();
    Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    Renderer::ClearScreen();

    VertexList vert_list;
    vert_list.push_back(CreateVertex(vec3(-0.5, -0.5, 0), vec2(0, 0)));
    vert_list.push_back(CreateVertex(vec3(0.5, -0.5, 0), vec2(1, 0)));
    vert_list.push_back(CreateVertex(vec3(0, 0.5, 0), vec2(0.5, 1)));

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
      ShaderVariable mvp_var = color_shader.uniform_var(kMVPHandleName);
      SetUniformMatrix(mvp_var, mvp);

      vec4 color(1.0f);
      ShaderVariable const_color_var = color_shader.uniform_var(kConstColorHandleName);
      SetUniformVector(const_color_var, color);
      SR_CHECK_ERROR("SetVector");

      color_shader.SetVertexList(vert_list);
      color_shader.DrawArrays(kDrawTriangles, vert_list.size());

      color_shader.DrawArrays(kDrawTriangles, vbo);

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
      ShaderVariable mvp_var = simple_shader.uniform_var(kMVPHandleName);
      SetUniformMatrix(mvp_var, mvp);
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
      ShaderVariable mvp_var = simple_shader.uniform_var(kMVPHandleName);
      SetUniformMatrix(mvp_var, mvp);
      SR_CHECK_ERROR("SetMatrix");

      GeometricObject<Vertex> mesh;
      //mesh.PointTeapot(0.05f);
      //mesh.WireTeapot(0.05f);
      //mesh.SolidTeapot(0.05f);
      //mesh.WireShpere(1, 16, 16);
      //mesh.PointShpere(1, 16, 16);
      //mesh.SolidSphere(1, 16, 16);
      //mesh.SolidCube(1, 1, 1);
      //mesh.WireCube(1, 1, 1);
      mesh.PointCube(1, 1, 1);
      //mesh.PointCylinder(1, 1, 2, 8, 8);
      //mesh.WireCylinder(1, 1, 2, 8, 8);
      mesh.SolidCylinder(1, 1, 2, 8, 8);
      //mesh.WireAxis(5);
      //mesh.SolidPlane(3);
      //mesh.WirePlane(3, 0.1f);
      //mesh.SolidTorus(1, 0.1);
      //mesh.SolidCone(2, 2);
      auto it = mesh.Begin();
      auto endit = mesh.End();
      for( ; it != endit ; ++it) {
        const DrawCmdData<Vertex> &cmd = *it;
        //앞면 뒷면 그리기를 허용/불가능 정보까지 내장해야
        //뚜껑없는 원통 그리기가 편하다
        if(cmd.disable_cull_face == true) {
          glDisable(GL_CULL_FACE);
        }
        simple_shader.SetVertexList(cmd.vertex_list);
        if(cmd.index_list.empty()) {
          simple_shader.DrawArrays(cmd.draw_mode, cmd.vertex_list.size());
        } else {
          simple_shader.DrawElements(cmd.draw_mode, cmd.index_list);
        }
        if(cmd.disable_cull_face == true) {
          glEnable(GL_CULL_FACE);
        }
      }
    }
    depth_fbo.Unbind();

    /*
    //fbo에 있는 내용을 적절히 그리기
    {
    device->render_device().Set2D();
    device->render_device().UseShader(simple_shader);
    ShaderVariable mvp_var = simple_shader.uniform_var(kMVPHandleName);
    mat4 world_mat(1.0f);
    SetUniformMatrix(mvp_var, world_mat);

    //device->render_device().UseTexture(depth_fbo.color_tex());
    device->render_device().UseTexture(depth_fbo.depth_tex());

    Vertex2DList vert_list;
    vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
    vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
    vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
    vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));
    simple_shader.SetVertexList(vert_list);
    simple_shader.DrawArrays(kDrawTriangleFan, vert_list.size());
    }
    */
    null_post_effect.DrawScissor(depth_fbo.color_tex(), &device->render_device(), 0, 0, 320, 480);
    grayscale_post_effect.DrawScissor(depth_fbo.color_tex(), &device->render_device(), 320, 0, 320, 480);
    grayscale_post_effect.Draw(depth_fbo.color_tex(), &device->render_device(), 100, 100, 100, 100);

    SR_CHECK_ERROR("End RenderFrame");
  }

  void update_frame(sora::Device *def, float dt) {

  }
} //namespace posteffect
} //namespace sora