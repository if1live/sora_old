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

#include "template_lib.h"
#include "touch_event.h"
#include "keyboard_event.h"
#include "filesystem.h"
#include "memory_file.h"

#include "render_device.h"

/*
#include "shader.h"
#include "mesh_manager.h"
#include "uber_shader.h"
#include "render_state.h"
#include "renderer.h"
#include "font.h"
#include "shader_bind_policy.h"
#include "material_manager.h"
#include "texture_manager.h"
*/

namespace sora {;

struct DevicePrivate {
  DevicePrivate(Device *dev)
    : render_device(dev) {
  }

  ~DevicePrivate() {
  }

  TouchEventQueue touch_evt_queue;
  KeyboardEventQueue keyboard_evt_queue;
  RenderDevice render_device;
};

Device::Device() : pimpl_(NULL) {
  pimpl_ = new DevicePrivate(this);
}

Device::~Device() {
  if(pimpl_ != NULL) {
    SafeDelete(pimpl_);
  }
}

RenderDevice &Device::render_device() {
  return pimpl().render_device;
}
TouchEventQueue &Device::touch_evt_queue() {
  return pimpl().touch_evt_queue;
}
KeyboardEventQueue &Device::keyboard_evt_queue() {
  return pimpl().keyboard_evt_queue;
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
  pimpl().render_device.EndRender();
}

}
