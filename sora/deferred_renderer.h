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
class Light;
class Shader;

class DeferredRenderer {
public:
  DeferredRenderer();
  ~DeferredRenderer();

  bool Init(int w, int h);
  void Deinit();

  //early z-pass의 구현을 위해서 화면을 간단하게 렌더링해서 깊이만 얻는다
  //early z-pass와 geometry pass는 기본적으로 같은 gbuffer를 공유한다
  //다만 렌더링할떄 z에만 렌더링/컬러에 렌더링을 분리해서 사용하게 된다
  void BeginDepthPass();
  void DrawDepthPass(Mesh *mesh);
  void EndDepthPass();

  void BeginGeometryPass();
  void EndGeometryPass();
  void DrawMesh(Mesh *mesh);

  void BeginLightPass();
  void DrawAmbientLight(const glm::vec3 &color);
  void DrawLight(const Light &light);
  void DrawDirectionalLight(const Light &light);
  void DrawPointLight(const Light &light);
  void DrawSpotLight(const Light &light);
  void DrawPointLightArea(const Light &light);
  void EndLightPass();

  GBuffer &gbuffer() { return *gbuffer_; }

  Texture DepthTex() const;
  Texture NormalTex() const;
  Texture DiffuseTex() const;
  Texture SpecularTex() const;

  Texture &FinalResultTex() const;

  uint GBufferHandle() const;

private:
  Shader &ambient_shader();
  Shader &directional_shader();
  Shader &point_shader(); //점광원용 쉐이더
  Shader &depth_shader();

private:
  std::unique_ptr<UberShader> geometry_uber_shader_;
  std::unique_ptr<Shader> depth_shader_;

  std::unique_ptr<GBuffer> gbuffer_;

  //최종 렌더링 결과가 될 fbo. 순수하게 색깔 데이터만 있어도 된다
  std::unique_ptr<FrameBuffer> final_result_fb_;

  std::unique_ptr<Shader> ambient_shader_;
  std::unique_ptr<Shader> directional_shader_;
  std::unique_ptr<Shader> point_shader_;
};

} //namespace sora

