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
#ifndef SORA_RENDER_DEVICE_H_
#define SORA_RENDER_DEVICE_H_

#include "renderer/gl/gl_render_device.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/sys_font.h"
#include "renderer/material.h"
#include "renderer/mesh_buffer.h"

namespace sora {;
class Device;
template<typename PolicyType> class RenderDeviceT;
typedef RenderDeviceT<sora::gl::GLRenderDevice> RenderDevice;

template<typename PolicyType>
class RenderDeviceT {
public:
  RenderDeviceT(Device *dev) : policy_(dev) {}
  RenderDeviceT() {}
  ~RenderDeviceT() {}

  void UseShader(Shader &shader) { policy_.UseShader(shader.handle()); }
  void UseTexture(Texture &tex) { policy_.UseTexture(tex.handle()); }

  void Set2D() { policy_.Set2D(); }
  void Set3D() { policy_.Set3D(); }

  void EndRender() { policy_.EndRender(); }
  void SetWinSize(int width, int height) { policy_.SetWinSize(width, height); }
  int win_width() const { return policy_.win_width(); }
  int win_height() const { return policy_.win_height(); }
  TextureManager &tex_mgr() { return tex_mgr_; }
  SysFont &sys_font() { return sys_font_; }
  MaterialManager &mtl_mgr() { return mtl_mgr_; }
  MeshBufferManager &mesh_mgr() { return mesh_mgr_; }

private:
  PolicyType policy_;
  TextureManager tex_mgr_;
  SysFont sys_font_;
  MaterialManager mtl_mgr_;
  MeshBufferManager mesh_mgr_;
};

}
#endif  // SORA_RENDER_DEVICE_H_