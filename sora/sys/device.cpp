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
#include "device.h"

#include <algorithm>

#include "renderer/material_manager.h"
#include "renderer/texture_manager.h"
#include "core/template_lib.h"
#include "event/touch_event.h"
#include "event/keyboard_event.h"
#include "renderer/mesh_manager.h"
#include "renderer/uber_shader.h"
#include "renderer/render_state.h"
#include "renderer/renderer.h"
#include "renderer/font.h"
#include "renderer/shader.h"
#include "sys/filesystem.h"
#include "sys/memory_file.h"
#include "renderer/shader_bind_policy.h"


namespace sora {;

struct DevicePrivate {
  DevicePrivate(Device *dev)
  : render_state(dev),
  render2d(dev), 
  render3d(dev) {
    Init();
  }

  ~DevicePrivate() {
    simple_shader.Deinit();
  }

  void Init() {
    render2d.SetPolicy_2D();
    light_uber_shader.Init<LightUberShaderLoadPolicy>();

    //init simple shader
    std::string app_vert_path = Filesystem::GetAppPath("shader/simple.vs");
    std::string app_frag_path = Filesystem::GetAppPath("shader/simple.fs");
    MemoryFile vert_file(app_vert_path);
    MemoryFile frag_file(app_frag_path);
    vert_file.Open();
    frag_file.Open();
    const char *vert_src = (const char*)(vert_file.start);
    const char *frag_src = (const char*)(frag_file.start);
    
    simple_shader.Init(vert_src, frag_src);

    ShaderNameBind *pos_bind = ShaderBindPolicy::GetPredefinedAttrib(ShaderBindPolicy::kPosition);
    ShaderNameBind *texcoord_bind = ShaderBindPolicy::GetPredefinedAttrib(ShaderBindPolicy::kTexcoord);
    ShaderNameBind *mvp_bind = ShaderBindPolicy::GetPredefinedUniform(ShaderBindPolicy::kWorldViewProjection);
    SR_ASSERT(pos_bind != NULL);
    SR_ASSERT(texcoord_bind != NULL);
    SR_ASSERT(mvp_bind != NULL);

    const ShaderVariable *pos_var = simple_shader.attrib_var(pos_bind->name);
    const ShaderVariable *texcoord_var = simple_shader.attrib_var(texcoord_bind->name);
    const ShaderVariable *mvp_var = simple_shader.uniform_var(mvp_bind->name);
    SR_ASSERT(pos_var != NULL);
    SR_ASSERT(texcoord_var != NULL);
    SR_ASSERT(mvp_var != NULL);


    ShaderBindPolicy &bind_policy = simple_shader.bind_policy;
    bind_policy.set_var(pos_bind->semantic, *pos_var);
    bind_policy.set_var(texcoord_bind->semantic, *texcoord_var);
    bind_policy.set_var(mvp_bind->semantic, *mvp_var);
  
    vert_file.Close();
    frag_file.Close();  
  }

  MaterialManager material_mgr;
  TextureManager texture_mgr;
  TouchEventQueue touch_evt_queue;
  KeyboardEventQueue keyboard_evt_queue;
  MeshManager mesh_mgr;
  UberShader light_uber_shader;
  RenderState render_state;
  ShaderProgram simple_shader;
  Renderer render2d;
  Renderer render3d;
  Font font;
};

Device::Device() : pimpl_(NULL) {
}

Device::~Device() {
  if(pimpl_ != NULL) {
    SafeDelete(pimpl_);
  }
}

Font &Device::font() {
  return pimpl().font;
}
Renderer &Device::render3d() {
  return pimpl().render3d;
}
Renderer &Device::render2d() {
  return pimpl().render2d;
}

TextureManager &Device::texture_mgr() {
  return pimpl().texture_mgr;
}
MaterialManager &Device::material_mgr() {
  return pimpl().material_mgr;
}
TouchEventQueue &Device::touch_evt_queue() {
  return pimpl().touch_evt_queue;
}
KeyboardEventQueue &Device::keyboard_evt_queue() {
  return pimpl().keyboard_evt_queue;
}

MeshManager &Device::mesh_mgr() {
  return pimpl().mesh_mgr;
}

ShaderProgram &Device::uber_shader(uint flag) {
  return pimpl().light_uber_shader.Load(flag);
}

RenderState &Device::render_state() {
  return pimpl().render_state;
}

ShaderProgram &Device::simple_shader() {
  return pimpl().simple_shader;
}
DevicePrivate &Device::pimpl() {
  if(pimpl_ == NULL) {
    pimpl_ = new DevicePrivate(this);
  }
  return *pimpl_;
}

void Device::EndTick() {
  pimpl().keyboard_evt_queue.Clear();
  pimpl().touch_evt_queue.Clear();
  Renderer::EndRender();
}

}
