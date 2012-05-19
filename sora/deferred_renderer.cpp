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
#include "sora_stdafx.h"
#include "deferred_renderer.h"
#include "shader.h"

#include "mesh_buffer.h"
#include "gbuffer.h"
#include "filesystem.h"

#include "device.h"
#include "render_state.h"
#include "material.h"
#include "texture.h"
#include "uber_shader.h"
#include "light.h"
#include "frame_buffer.h"

using namespace std;
using namespace glm;

namespace sora {;

DeferredRenderer::DeferredRenderer() {
}
DeferredRenderer::~DeferredRenderer() {
  Deinit();
}
bool DeferredRenderer::Init(int w, int h) {
  SR_ASSERT(geometry_uber_shader_.get() == NULL);
  SR_ASSERT(depth_shader_.get() == NULL);
  SR_ASSERT(gbuffer_.get() == NULL);
  SR_ASSERT(final_result_fb_.get() == NULL);
  SR_ASSERT(ambient_shader_.get() == NULL);

  gbuffer_ = move(unique_ptr<GBuffer>(new GBuffer()));
  gbuffer_->Init(w, h);

  LOGI("Deferred Renderer :: Geomerty Shader");
  const char *vert_file = "shader/deferred_geometry.vs";
  const char *frag_file = "shader/deferred_geometry.fs";
  unsigned int flag = 0;
  //flag |= kMaterialAmbient; //디퍼드는 객체별 ambient가 먹히지 않는다(속성을 따로 저장하지 않아서)
  flag |= kMaterialDiffuse;
  flag |= kMaterialSpecular;
  flag |= kMaterialDiffuseMap;
  flag |= kMaterialSpecularMap;
  //normal map
  flag |= kMaterialNormalMap;

  geometry_uber_shader_.reset(new UberShader());
  geometry_uber_shader_->InitWithFile(vert_file, frag_file, flag);

  //depth pass용으로 쓰기 위한 쉐이더
  LOGI("Deferred Renderer :: Depth Shader");
  const char *depth_vert_file = "shader/deferred_depth.vs";
  const char *depth_frag_file = "shader/deferred_depth.fs";
  depth_shader_.reset(new Shader());
  depth_shader_->LoadFromFile(depth_vert_file, depth_frag_file);

  //최종결과는 색으로 충분
  final_result_fb_.reset(new FrameBuffer());
  final_result_fb_->InitWithoutDepth(w, h);

  LOGI("Deferred Renderer :: Ambient Shader");
  const char *ambient_vert_file = "shader/deferred_ambient_light.vs";
  const char *ambient_frag_file = "shader/deferred_ambient_light.fs";
  ambient_shader_.reset(new Shader());
  ambient_shader_->LoadFromFile(ambient_vert_file, ambient_frag_file);

  return true;
}
void DeferredRenderer::Deinit() {
  if(gbuffer_.get() != NULL) {
    gbuffer_->Deinit();
    gbuffer_.reset(NULL);
  }
  if(geometry_uber_shader_.get() != NULL) {
    geometry_uber_shader_->Deinit();
    geometry_uber_shader_.reset(NULL);
  }
  if(depth_shader_.get() != NULL) {
    depth_shader_->Deinit();
    depth_shader_.reset(NULL);
  }
  if(final_result_fb_.get() != NULL) {
    final_result_fb_->Deinit();
    final_result_fb_.reset(NULL);
  }
  if(ambient_shader_.get() != NULL) {
    ambient_shader_->Deinit();
    ambient_shader_.reset(NULL);
  }
}

void DeferredRenderer::BeginDepthPass() {
  Device *dev = Device::GetInstance();
  RenderState &render_state = dev->render_state();

  //gbuffer_->Bind(); //early z-pass와 지오메트리 패스는 gbuffer 를 공유한다
  //그래서 gbuffer바인딩을 먼저해준다
  gbuffer_->Bind();
  vec4ub color(0, 0, 0, 255);
  render_state.ClearBuffer(true, true, false, color);
  render_state.Set3D();

  glColorMask(false, false, false, false);
  glDepthMask(true);

  //쉐이더는 깊이 렌더링중에 교체되지 않는다
  render_state.UseShader(*depth_shader_);
}
void DeferredRenderer::DrawDepthPass(Mesh *mesh) {
  Device *dev = Device::GetInstance();
  RenderState &render_state = dev->render_state();
  //mesh에 따라서 model 행렬은 변하니까 begin같은곳에 떄려박지 못하고
  //메시마다 따로 대입한다
  mat4 mvp = render_state.GetMVPMatrix();
  depth_shader_->SetUniformMatrix(kMVPHandleName, mvp);
  depth_shader_->DrawMeshIgnoreMaterial(mesh);
}
void DeferredRenderer::EndDepthPass() {
  glColorMask(true, true, true, true);
  glDepthMask(false);
}

void DeferredRenderer::BeginGeometryPass() {
}
void DeferredRenderer::EndGeometryPass() {
  gbuffer_->Unbind();
  glDepthMask(true);
}

void DeferredRenderer::DrawMesh(Mesh *mesh) {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  const Material &material = render_state.LastMaterial();
  unsigned int shader_flag = material.props;
  Shader &shader = geometry_uber_shader_->Load(shader_flag);

  //물체그릴떄마다 재질이 바뀌면 쉐이더도 바뀌니까 적절히 쉐이더 속성 같이 설정해주기
  render_state.UseShader(shader);
  geometry_uber_shader_->ApplyCamera();
  geometry_uber_shader_->ApplyMaterial(material);

  shader.DrawMeshIgnoreMaterial(mesh);
}

Texture DeferredRenderer::DepthTex() const {
  return gbuffer_->DepthTex();
}
Texture DeferredRenderer::NormalTex() const {
  return gbuffer_->NormalTex();
}
Texture DeferredRenderer::DiffuseTex() const {
  return gbuffer_->DiffuseTex();
}
Texture DeferredRenderer::SpecularTex() const {
  return gbuffer_->SpecularTex();
}
Texture DeferredRenderer::PositionTex() const {
  return gbuffer_->PositionTex();
}


void DeferredRenderer::BeginLightPass() {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  render_state.Set2D();

  final_result_fb_->Bind();

  vec4ub color(0, 0, 0, 255);
  render_state.ClearBuffer(true, false, false, color);
}
void DeferredRenderer::EndLightPass() {
  final_result_fb_->Unbind();
}
void DeferredRenderer::DrawLight(const Light &light) {
  switch(light.type) {
  case kLightDirection:
    DrawDirectionalLight(light);
    break;
  case kLightPoint:
    DrawPointLight(light);
    break;
  case kLightSpotLight:
    DrawSpotLight(light);
    break;
  default:
    SR_ASSERT(!"not valid");
  }
}
void DeferredRenderer::DrawDirectionalLight(const Light &light) {
  SR_ASSERT(light.type == kLightDirection);
}
void DeferredRenderer::DrawPointLight(const Light &light) {
  SR_ASSERT(light.type == kLightPoint);
}
void DeferredRenderer::DrawSpotLight(const Light &light) {
  SR_ASSERT(light.type == kLightSpotLight);
}
void DeferredRenderer::DrawAmbientLight(const glm::vec3 &color) {
  //화면 전체를 단색으로 그리기
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  render_state.UseShader(*ambient_shader_);

  mat4 mvp(1.0f);
  ambient_shader_->SetUniformMatrix(kMVPHandleName, mvp);

  vec4 ambient_color(color, 1.0f);
  ambient_shader_->SetUniformVector(kAmbientColorHandleName, ambient_color);

  Texture diffuse_tex = DiffuseTex();
  render_state.UseTexture(diffuse_tex, 0);
  
  vector<Vertex2D> vert_list;
  vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
  vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
  vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));

  ambient_shader_->DrawArrays(kDrawTriangleFan, vert_list);
}
unsigned int DeferredRenderer::GBufferHandle() const {
  return gbuffer_->fbo();
}
Texture &DeferredRenderer::FinalResultTex() const {
  return final_result_fb_->color_tex();
}
} //namespace sora