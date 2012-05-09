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

#include "globals.h"

namespace sora {;
struct RenderDeviceInterface;

class RenderDevice {
public:
  RenderDevice();
  ~RenderDevice();

  void UseShader(Shader &shader);
  void UseTexture(Texture &tex);

  void Set2D();
  void Set3D();

  void EndRender();

  void SetWinSize(int width, int height);

  int win_width() const { return win_width_; }
  int win_height() const { return win_height_; }

private:
  RenderDeviceInterface *policy_;
  int win_width_;
  int win_height_;
};

struct RenderDeviceInterface {
  RenderDeviceInterface() {}
  virtual ~RenderDeviceInterface() {}

  virtual void UseShader(Shader &shader) = 0;
  virtual void UseTexture(Texture &tex) = 0;

  virtual void Set2D() = 0;
  virtual void Set3D() = 0;

  virtual void EndRender() = 0;

  virtual void SetWinSize(int width, int height) = 0;
};
}
#endif  // SORA_RENDER_DEVICE_H_