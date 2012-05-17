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
class GBuffer;
class Mesh;
class Material;
class UberShader;

class DeferredRenderer {
public:
  DeferredRenderer();
  ~DeferredRenderer();

  bool Init(int w, int h);
  void Deinit();

  void BeginGeometryPass();
  void EndGeometryPass();
  void ApplyGeomertyPassRenderState();
  void DrawMesh(Mesh *mesh);

  GBuffer &gbuffer() { return *gbuffer_; }

  Texture DepthTex() const;
  Texture NormalTex() const;
  Texture DiffuseTex() const;
  Texture SpecularTex() const;

private:
  //std::unique_ptr<Shader> geometry_shader_;
  std::unique_ptr<UberShader> geometry_uber_shader_;

  std::unique_ptr<GBuffer> gbuffer_;
};

} //namespace sora
