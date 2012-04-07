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
class MeshManager;
class UberShader;
class Renderer;
class Font;
class ShaderProgram;

//for event
struct TouchEvent;
struct KeyboardEvent;
template<typename T>  class EventQueue;
typedef EventQueue<TouchEvent> TouchEventQueue;
typedef EventQueue<KeyboardEvent> KeyboardEventQueue;

struct RenderState;

class Device {
public:
  Device();
  ~Device();

  Font &font();

  RenderState &render_state();
  TextureManager &texture_mgr();
  MaterialManager &material_mgr();
  MeshManager &mesh_mgr();

  TouchEventQueue &touch_evt_queue();
  KeyboardEventQueue &keyboard_evt_queue();
  
  //uber shader + predefined shader
  ShaderProgram &uber_shader(uint flag);
  ShaderProgram &simple_shader(); //단순 2d용

  static Device *GetAnyDevice();

  Renderer &render3d();
  Renderer &render2d();

  //한 프레임이 완료된후에 리셋할 정보를 적절히 리셋하기
  void EndTick();

private:
  DevicePrivate &pimpl();
  mutable DevicePrivate *pimpl_;
};
}

#endif  // SORA_RENDER_DEVICE_H_