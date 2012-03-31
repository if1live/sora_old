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
#ifndef SORA_GL_VIEW_H_
#define SORA_GL_VIEW_H_

namespace sora {;
class Device;
}

namespace sora {;
public ref class GLView abstract {
public:
  GLView();
  virtual ~GLView();

  virtual void SetupGraphics(int w, int h) = 0;
  virtual void SetWindowSize(int w, int h) = 0;
  virtual void DrawFrame() = 0;
  virtual void InitGLEnv() = 0;
  virtual void UpdateFrame(float dt) = 0;
  virtual void Cleanup() = 0;

protected:
  sora::Device &device();
private:
  sora::Device *device_;
};
}

#endif  // SORA_GL_VIEW_H_