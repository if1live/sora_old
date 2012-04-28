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
#pragma  once

#include "renderer/gl/gl_renderer.h"
namespace sora {;
template<typename PolicyType> class RendererT;
typedef sora::gl::GLRenderer RendererPolicy;
typedef RendererT<RendererPolicy> Renderer;

template<typename PolicyType>
class RendererT : public PolicyType {
public:
  typedef PolicyType Policy;
public:
  static void SetClearColor(float r, float g, float b, float a) {
    ctx.clear_color_[0] = r;
    ctx.clear_color_[1] = g;
    ctx.clear_color_[2] = b;
    ctx.clear_color_[3] = a;
    Policy::SetClearColor(r, g, b, a);
  }
  static void ClearScreen() {
    Policy::ClearScreen();
  }
private:
  RendererT() { }
  static RendererT ctx;

  glm::vec4 clear_color_;
};

template<typename PolicyType>
RendererT<PolicyType> RendererT<PolicyType>::ctx;

}
