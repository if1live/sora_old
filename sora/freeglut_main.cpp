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
#include "freeglut_main.h"

#include "renderer/shader.h"
#include "sys/device.h"
#include "renderer/renderer_env.h"
#include "renderer/render_device.h"
#include "mesh/freeglut_font.h"
#include "sys/filesystem.h"

using namespace std;
using namespace glm;

namespace sora {;
namespace freeglut {

  Shader color_shader;

  void setup_graphics(sora::Device *dev, int w, int h) {
    dev->render_device().SetWinSize(w, h);

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
      //freeglut로 폰트 렌더링 하는 경우, 선그리기같은 느낌이니까
      //단색 쉐이더만 잇으면 충분하다
      string color_vs_path = Filesystem::GetAppPath("shader/const_color.vs");
      string color_fs_path = Filesystem::GetAppPath("shader/const_color.fs");
      color_shader.LoadFromFile(color_vs_path, color_fs_path);
    }
  }
  void draw_frame(sora::Device *dev) {
    SR_CHECK_ERROR("Render 2d start");
    //텍스트는 3d에도 찍을수 잇겟지만 지금은 2d로 충분하니까 2D로 렌더링함
    //glut font는 디버깅이라든가 그런 용도에 쓰이게 될 것이다
    dev->render_device().Set2D();
    
    //단색으로 glut용 그리기
    dev->render_device().UseShader(color_shader);
    sora::Shader &shader = color_shader;

    float win_width = (float)dev->render_device().win_width();
    float win_height = (float)dev->render_device().win_height();
    glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);

    for(int i = 0 ; i < 2 ; i++) {
      mat4 world_mat(1.0f);
      mat4 mvp(1.0f);

      vector< DrawCmdData<vec2> > font_vert_data;

      if(i == 0) {
        vec4 color(1.0f, 0, 0, 1.0f);
        shader.SetVector(kConstColorHandleName, color);

        world_mat = glm::translate(world_mat, glm::vec3(200, 200, 0));
        mat4 mvp = projection * world_mat;
        shader.SetMatrix(kMVPHandleName, mvp);
        font_vert_data = glutStrokeString(GLUT_STROKE_MONO_ROMAN, "ABCD");

      } else {
        vec4 color(0.0f, 1.0f, 0, 1.0f);
        shader.SetVector(kConstColorHandleName, color);

        world_mat = glm::translate(world_mat, glm::vec3(100, 100, 0));
        mat4 mvp = projection * world_mat;
        shader.SetMatrix(kMVPHandleName, mvp);
        font_vert_data = glutStrokeCharacter(GLUT_STROKE_ROMAN, '@');
      }
      
      auto it = font_vert_data.begin();
      auto endit = font_vert_data.end();
      
      glLineWidth(3.0f);
      for( ; it != endit ; ++it) {
        const DrawCmdData<vec2> &cmd = *it;
        if(cmd.disable_cull_face == true) {
          glDisable(GL_CULL_FACE);
        }
        shader.SetVertexList(cmd.vertex_list);
        if(cmd.index_list.empty()) {
          shader.DrawArrays(cmd.draw_mode, cmd.vertex_list.size());
        } else {
          shader.DrawElements(cmd.draw_mode, cmd.index_list);
        }
        if(cmd.disable_cull_face == true) {
          glEnable(GL_CULL_FACE);
        }
      }
      glLineWidth(1.0f);
    }
  }
  void update_frame(sora::Device *def, float dt) {

  }
}
}
