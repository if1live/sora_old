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
#include "device.h"

#include "template_lib.h"
#include "touch_event.h"
#include "keyboard_event.h"
#include "filesystem.h"
#include "memory_file.h"

#include "render_state.h"

#include "mesh.h"
#include "material.h"
#include "texture.h"
#include "sys_font.h"

#include "draw_2d_manager.h"
#include "debug_draw_manager.h"
#include "shader_manager.h"

using namespace std;

namespace sora {;

Device::Device()
: touch_evt_queue_(new TouchEventQueue()),
keyboard_evt_queue_(new KeyboardEventQueue()),
mesh_mgr_(new MeshManager()),
tex_mgr_(new TextureManager()),
mtl_mgr_(new MaterialManager()),
sys_font_(new SysFont()),
render_state_(new RenderState()),
draw_2d_(new Draw2DManager()),
debug_draw_mgr_(new DebugDrawManager()),
shader_mgr_(new ShaderManager()) {
  shader_mgr_->SetUp();
}

Device::~Device() {
}

void Device::EndTick() {
  keyboard_evt_queue_->Clear();
  touch_evt_queue_->Clear();
  render_state_->EndRender();
}

Device *Device::GetInstance() {
  SR_ASSERT(dev_ != nullptr && "not created yet!");
  return dev_;
}
bool Device::CreateDevice() {
  SR_ASSERT(dev_ == nullptr && "already created");
  if(dev_ == NULL) {
    dev_ = new Device();
    return true;
  } else {
    return false;
  }
}
bool Device::DestroyDevice() {
  SR_ASSERT(dev_ != nullptr && "device not exist");
  if(dev_ != nullptr) {
    delete(dev_);
    dev_ = NULL;
    return true;
  } else {
    return false;
  }
}

Device *Device::dev_ = nullptr;
}
