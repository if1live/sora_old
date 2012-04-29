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

#include "renderer/globals.h"
#include "renderer/material.h"
#include "renderer/light.h"

#include "renderer/shader_variable.h"
#include "renderer/uber_shader.h"

namespace sora {;
struct Material;
struct Light;
struct Camera;
//기본적인 uber shader에 대응하기 위한 렌더러
//이거로 기본 인터페이스를 테스트하고 인터페이스를 뽑아내는식으로 하면 될듯?
namespace gl {
  class GLUberShaderRenderer {
  public:
    GLUberShaderRenderer();
    ~GLUberShaderRenderer();
    void Init();
    void SetMaterial(const Material &mtl);
    void SetLight(const Light &light);
    void ApplyMaterialLight(RenderDevice *render_dev);
    void SetCamera(const Camera &cam, const glm::mat4 &model, RenderDevice *render_dev);
    void SetCamera(const Camera &cam, RenderDevice *render_dev);

    Shader &GetCurrShader();
  private:
    Material material_;
    Light light_;
    UberShader uber_shader_;

  };
} //namespace gl
} //namespace sora
