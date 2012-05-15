﻿/*  Copyright (C) 2011-2012 by if1live */
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
class Mesh;
class Light;
class Material;

namespace gl {
  class GLUberShaderRenderer;
};


class ForwardRenderer {
public:
  ForwardRenderer();
  ~ForwardRenderer();

  void Init();
  void Deinit();

  void BeginPass();
  void EndPass();
  void ApplyRenderState();
  void DrawMesh(Mesh *mesh);

  void SetMaterial(const Material &mtl);
  void SetLight(const Light &light);

private:
  //uber shader기반으로 forware만들기
  std::unique_ptr<sora::gl::GLUberShaderRenderer> uber_renderer_;
};
} //namespace sora