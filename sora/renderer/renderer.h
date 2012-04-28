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
#include "renderer/texture.h"

namespace sora {;
template<typename PolicyType> class RendererT;
typedef sora::gl::GLRenderer RendererPolicy;
typedef RendererT<RendererPolicy> Renderer;

typedef unsigned int FBOHandleType;

template<typename PolicyType>
class RendererT : public PolicyType {
public:
  typedef PolicyType Policy;
public:
  static void SetClearColor(float r, float g, float b, float a) {
    SR_ASSERT(ctx.inited_ == true);
    ctx.clear_color_[0] = r;
    ctx.clear_color_[1] = g;
    ctx.clear_color_[2] = b;
    ctx.clear_color_[3] = a;
    Policy::SetClearColor(r, g, b, a);
  }
  static void ClearScreen() {
    SR_ASSERT(ctx.inited_ == true);
    Policy::ClearScreen();
  }
  static void Init(int w, int h) {
    SR_ASSERT(ctx.inited_ == false);
    ctx.inited_ = true;
    Policy::InitColorTexture(w, h, &ctx.color_tex_);
    Policy::InitDepthTexture(w, h, &ctx.depth_tex_);
    Policy::InitFBO(&ctx.fbo_, ctx.color_tex_, ctx.depth_tex_);
  }

  static void BindFBO() {
    SR_ASSERT(ctx.inited_ == true);
    Policy::BindFBO(ctx.fbo_);
  }
  static void UnbindFBO() {
    SR_ASSERT(ctx.inited_ == true);
    Policy::UnbindFBO();
  }

  static Texture &depth_tex() {
    SR_ASSERT(ctx.inited_ == true);
    return ctx.depth_tex_;
  }
  static Texture &color_tex() {
    SR_ASSERT(ctx.inited_ == true);
    return ctx.color_tex_;
  }

private:
  RendererT() : fbo_(0), inited_(false) { }

  static RendererT ctx;

  glm::vec4 clear_color_;
  
  bool inited_;
  FBOHandleType fbo_;
  Texture depth_tex_;
  Texture color_tex_;
};

template<typename PolicyType>
RendererT<PolicyType> RendererT<PolicyType>::ctx;

}
