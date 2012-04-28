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
#include "renderer/gl/gl_frame_buffer.h"
#include "renderer/texture.h"

namespace sora {;
template<typename PolicyType> class FrameBufferT;
typedef sora::gl::GLFrameBuffer FrameBufferPolicy;
typedef FrameBufferT<FrameBufferPolicy> FrameBuffer;

template<typename PolicyType>
class FrameBufferT : public PolicyType {
public:
 typedef PolicyType Policy;
 typedef typename Policy::HandleType HandleType;
public:
  FrameBufferT() : handle_(0) {}
  ~FrameBufferT() {}
  //깊이 테스트용으로 쓰기 위한 초기화함수
  //깊이텍스쳐의 정밀도 높음. 색깔 텍스쳐 정밀도는 낮음
  void InitAsDepthTex(int w, int h) {
    Policy::InitAsDepthTex(&handle_, w, h, &color_tex_, &depth_tex_);
  }
  void Deinit() { Policy::Deinit(&handle_); }
  bool IsInit() const { return Policy::IsInit(handle_); }

  Texture &color_tex() { return color_tex_; }
  Texture &depth_tex() { return depth_tex_; }

  void Bind() { Policy::Bind(handle_); }
  void Unbind() { Policy::Unbind(); }

private:
  HandleType handle_;
  Texture color_tex_;
  Texture depth_tex_;
};
}
