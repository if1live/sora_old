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

#include "renderer/material_manager.h"
#include "renderer/texture_manager.h"
#include "core/template_lib.h"
#include "event/touch_event.h"
#include "renderer/mesh_manager.h"
#include "renderer/uber_shader.h"
#include "renderer/render_state.h"
#include "renderer/renderer.h"
#include "renderer/font.h"

namespace sora {;

struct DevicePrivate {
  DevicePrivate(Device *dev)
  : render_state(dev),
  render2d(dev), 
  render3d(dev) {
    render2d.SetPolicy_2D();
    simple_uber_shader.Init<SimpleUberShaderLoadPolicy>();
    light_uber_shader.Init<LightUberShaderLoadPolicy>();
  }
  DevicePrivate(const Device *dev)
  : render_state(const_cast<Device*>(dev)),
  render2d(const_cast<Device*>(dev)), 
  render3d(const_cast<Device*>(dev)) {
    render2d.SetPolicy_2D();
    simple_uber_shader.Init<SimpleUberShaderLoadPolicy>();
    light_uber_shader.Init<LightUberShaderLoadPolicy>();
  }

  MaterialManager material_mgr;
  TextureManager texture_mgr;
  TouchEventQueue touch_evt_queue;
  MeshManager mesh_mgr;
  UberShader simple_uber_shader;
  UberShader light_uber_shader;
  RenderState render_state;
  Renderer render2d;
  Renderer render3d;
  Font font;
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
UberShader &Device::simple_uber_shader() {
  return pimpl().simple_uber_shader;
}
const UberShader &Device::simple_uber_shader() const {
  return pimpl().simple_uber_shader;
}
UberShader &Device::light_uber_shader() {
  return pimpl().light_uber_shader;
}
const UberShader &Device::light_uber_shader() const {
  return pimpl().light_uber_shader;
}

ShaderProgram &Device::uber_shader(uint flag) {
  if(light_uber_shader().IsValidFlag(flag) == true) {
    return light_uber_shader().Load(flag);
  }
  if(simple_uber_shader().IsValidFlag(flag) == true) {
    return simple_uber_shader().Load(flag);
  }
  //적절한것이 존재하지 않는 경우, 빛을 기본값으로 사용하자
  return light_uber_shader().Load(flag);
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