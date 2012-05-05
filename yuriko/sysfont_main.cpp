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

#include "shader.h"
#include "device.h"
#include "renderer_env.h"
#include "render_device.h"
#include "filesystem.h"
#include "vertex.h"
#include "debug_draw_manager.h"

using namespace std;
using namespace glm;

namespace sora {;
namespace sysfont {

  Shader simple_shader;

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
      //create shader
      string simple_vs_path = Filesystem::GetAppPath("shader/simple.vs");
      string simple_fs_path = Filesystem::GetAppPath("shader/simple.fs");
      simple_shader.LoadFromFile(simple_vs_path, simple_fs_path);
    }
  }

  void draw_frame(sora::Device *device) {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    SR_CHECK_ERROR("Render 2d start");
    device->render_device().Set2D();

    device->render_device().UseShader(simple_shader);
    sora::SysFont &font = device->render_device().sys_font();
    device->render_device().UseTexture(font.font_texture());

    //해상도에 맞춰서 적절히 설정
    float win_width = (float)device->render_device().win_width();
    float win_height = (float)device->render_device().win_height();
    glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
    ShaderVariable mvp_var = simple_shader.uniform_var(kMVPHandleName);
    SetUniformMatrix(mvp_var, projection);
    */

    /*
    Vertex2DList vert_list;
    vert_list.push_back(CreateVertex2D(100, 100, 0, 1));
    vert_list.push_back(CreateVertex2D(100+128*2, 100, 1, 1));
    vert_list.push_back(CreateVertex2D(100+128*2, 100+128*2, 1, 0));
    vert_list.push_back(CreateVertex2D(100, 100+128*2, 0, 0));
    simple_shader.SetVertexList(vert_list);
    simple_shader.DrawArrays(kDrawTriangleFan, vert_list.size());
    */
    /*
    mat4 world_mat(1.0f);
    world_mat = glm::translate(world_mat, glm::vec3(0, 480, 0));
    world_mat = glm::scale(world_mat, glm::vec3(2, 2, 1));
    mat4 mvp = projection * world_mat;
    SetUniformMatrix(mvp_var, mvp);
    sora::Label label(&font, "PQRS_1234_asdf");
    simple_shader.SetVertexList(label.vertex_list());
    simple_shader.DrawElements(kDrawTriangles, label.index_list());
    */

    //디버깅용으로 화면 2d좌표계에 렌더링 하는거
    DebugDrawManager::Get2D().AddString(vec3(100, 100, 0), "asd", Color_Red(), 2.0f);
    DebugDrawPolicy_2D debug_draw;
    debug_draw.Draw(DebugDrawManager::Get2D(), &device->render_device());
    DebugDrawManager::Get2D().Clear();
  }
  void update_frame(sora::Device *def, float dt) {

  }
}
}
