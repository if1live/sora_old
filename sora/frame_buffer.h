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
#pragma once

#include "globals.h"
#include "gl_frame_buffer.h"
#include "texture.h"

namespace sora {;

template<typename PolicyType>
class FrameBufferT : public PolicyType {
public:
 typedef PolicyType Policy;
 typedef FrameBufferHandle HandleType;
public:
  FrameBufferT() : handle_(0), width_(0), height_(0) {}
  ~FrameBufferT() {}
  
  //표준형 fbo, 깊이16bit, 8bit rgba
  void Init(int w, int h) {
    Policy::Init(&handle_, w, h, &color_tex_, &depth_tex_);
  }
  void InitWithoutDepth(int w, int h) {
    Policy::InitWithoutDepth(&handle_, w, h, &color_tex_);
  }

  void Deinit() {
    if(color_tex_.handle() != 0) {
      color_tex_.Deinit();
    }
    if(depth_tex_.handle() != 0) {
      depth_tex_.Deinit();
    }
    Policy::Deinit(&handle_); 
  }
  bool IsInit() const { return Policy::IsInit(handle_); }

  Texture &color_tex() { return color_tex_; }
  Texture &depth_tex() { return depth_tex_; }

  void Bind() { Policy::Bind(handle_); }
  void Unbind() { Policy::Unbind(); }

  int width() const { return width_; }
  int height() const { return height_; }

private:
  HandleType handle_;
  Texture color_tex_;
  Texture depth_tex_;
  int width_;
  int height_;
};
}
