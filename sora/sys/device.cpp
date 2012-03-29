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

#include "renderer/material_manager.h"
#include "renderer/texture_manager.h"
#include "core/template_lib.h"
#include "event/touch_event.h"
#include "renderer/mesh_manager.h"
#include "renderer/uber_shader.h"
#include "renderer/render_state.h"

namespace sora {;

struct DevicePrivate {
  DevicePrivate(Device *dev)
  : render_state(dev) {
  }
  DevicePrivate(const Device *dev)
  : render_state(const_cast<Device*>(dev)) {
  }
  MaterialManager material_mgr;
  TextureManager texture_mgr;
  TouchEventQueue touch_evt_queue;
  MeshManager mesh_mgr;
  UberShader uber_shader;
  RenderState render_state;
};

Device::Device() : pimpl_(NULL) {
  device_list_.push_back(this);
}

Device::~Device() {
  if(pimpl_ != NULL) {
    SafeDelete(pimpl_);
  }

  auto found = std::find(device_list_.begin(), device_list_.end(), this);
  SR_ASSERT(found != device_list_.end());
  device_list_.erase(found);
}

Device *Device::GetAnyDevice() {
  if(device_list_.empty()) {
    return NULL;
  } else {
    return device_list_.front();
  }
}
std::vector<Device*> Device::device_list_;

TextureManager &Device::texture_mgr() {
  return pimpl().texture_mgr;
}
const TextureManager &Device::texture_mgr() const {
  return pimpl().texture_mgr;
}

MaterialManager &Device::material_mgr() {
  return pimpl().material_mgr;
}
const MaterialManager &Device::material_mgr() const {
  return pimpl().material_mgr;
}

TouchEventQueue &Device::touch_evt_queue() {
  return pimpl().touch_evt_queue;
}
const TouchEventQueue &Device::touch_evt_queue() const {
  return pimpl().touch_evt_queue;
}

MeshManager &Device::mesh_mgr() {
  return pimpl().mesh_mgr;
}
const MeshManager &Device::mesh_mgr() const {
  return pimpl().mesh_mgr;
}
UberShader &Device::uber_shader() {
  return pimpl().uber_shader;
}
const UberShader &Device::uber_shader() const {
  return pimpl().uber_shader;
}

RenderState &Device::render_state() {
  return pimpl().render_state;
}
const RenderState &Device::render_state() const {
  return pimpl().render_state;
}

DevicePrivate &Device::pimpl() {
  if(pimpl_ == NULL) {
    pimpl_ = new DevicePrivate(this);
  }
  return *pimpl_;
}
const DevicePrivate &Device::pimpl() const {
  if(pimpl_ == NULL) {
    pimpl_ = new DevicePrivate(this);
  }
  return *pimpl_;
}
}
