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
#ifndef SORA_DEVICE_H_
#define SORA_DEVICE_H_

#include <vector>

namespace sora {;

struct DevicePrivate;

class TextureManager;
class MaterialManager;
class TouchEventQueue;
class MeshManager;
class UberShader;
class Renderer;
class Font;
class ShaderProgram;

struct RenderState;

class Device {
public:
  Device();
  ~Device();

  Font &font();

  RenderState &render_state();
  const RenderState &render_state() const;

  TextureManager &texture_mgr();
  const TextureManager &texture_mgr() const;
  MaterialManager &material_mgr();
  const MaterialManager &material_mgr() const;
  MeshManager &mesh_mgr();
  const MeshManager &mesh_mgr() const;

  TouchEventQueue &touch_evt_queue();
  const TouchEventQueue &touch_evt_queue() const;
  
  //uber shader + predefined shader
  ShaderProgram &uber_shader(uint flag);
  ShaderProgram &simple_shader(); //단순 2d용

  static Device *GetAnyDevice();

  Renderer &render3d();
  Renderer &render2d();

private:
  DevicePrivate &pimpl();
  const DevicePrivate &pimpl() const;
  mutable DevicePrivate *pimpl_;
};
}

#endif  // SORA_RENDER_DEVICE_H_