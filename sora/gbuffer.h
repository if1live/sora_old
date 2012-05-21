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

namespace sora {;

class GBuffer {
public:
  enum {
    kDepthTex,
    kNormalTex,
    kDiffuseTex,
    kSpecularTex,
    kPositionTex,
    kTexCount
  };

  GBuffer();
  ~GBuffer();

  bool Init(int w, int h);
  bool IsInit() const;
  void Deinit();

  void Bind();
  void Unbind();

  Texture DepthTex() const;
  Texture NormalTex() const;
  Texture DiffuseTex() const;
  Texture SpecularTex() const;
  Texture PositionTex() const;

  FrameBufferHandle fbo() const { return fbo_; }

private:
  Texture GetTex(int tex_code) const;

  FrameBufferHandle fbo_;
  //depth texture(그냥 깊이버퍼)
  //RT0, view-space normal
  //RT1, diffuse(color * albedo)
  //RT2, specular map(rgb + shininess)
  //RT3, viewspace-position map(xyz. 역계산하는게 빡쳐서 그냥 RT로 때우자. 나중에 최적화하자)
  std::array<TextureHandle, kTexCount> tex_list_;

  int width_;
  int height_;

  uint stencil_;
};

} //namespace sora