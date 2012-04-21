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

#include "core/template_lib.h"
#include "event/touch_event.h"
#include "event/keyboard_event.h"
#include "sys/filesystem.h"
#include "sys/memory_file.h"

#include "renderer/render_device.h"

/*
#include "renderer/shader.h"
#include "renderer/mesh_manager.h"
#include "renderer/uber_shader.h"
#include "renderer/render_state.h"
#include "renderer/renderer.h"
#include "renderer/font.h"
#include "renderer/shader_bind_policy.h"
#include "renderer/material_manager.h"
#include "renderer/texture_manager.h"
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
