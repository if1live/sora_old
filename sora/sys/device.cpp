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

namespace sora {;

struct DevicePrivate {
  MaterialManager material_mgr;
  TextureManager texture_mgr;
};

Device::Device() : pimpl_(NULL) {
}

Device::~Device() {
  if(pimpl_ != NULL) {
    SafeDelete(pimpl_);
  }
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

DevicePrivate &Device::pimpl() {
  if(pimpl_ == NULL) {
    pimpl_ = new DevicePrivate();
  }
  return *pimpl_;
}
const DevicePrivate &Device::pimpl() const {
  if(pimpl_ == NULL) {
    pimpl_ = new DevicePrivate();
  }
  return *pimpl_;
}
}
