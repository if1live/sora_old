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
#include "draw_2d_manager.h"

using namespace std;
using namespace glm;

namespace sora {;
namespace sysfont {

  Draw2DManager mgr_2d;

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
  }

  void draw_frame(sora::Device *device) {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //디버깅용으로 화면 2d좌표계에 렌더링 하는거
    mgr_2d.AddString(vec2(100, 100), "asd", Color_Red(), 2.0f);
    mgr_2d.AddLine(vec2(100, 100), vec2(150, 200), Color_Blue(), 4.0f);
    //mgr_2d.AddSphere(vec2(200, 200, 0), 30, Color_Green());
    mgr_2d.AddCross(vec2(200, 200), Color_Green(), 5);

    sora::Draw2DPolicy draw_policy;
    draw_policy.Draw(mgr_2d, &device->render_device());
  }
  void update_frame(sora::Device *def, float dt) {
    mgr_2d.Update(dt);
  }
}
}
