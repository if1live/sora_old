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

using namespace std;
using namespace glm;

namespace sora {;

DeferredRenderer::DeferredRenderer() {
}
DeferredRenderer::~DeferredRenderer() {
  Deinit();
}
bool DeferredRenderer::Init(int w, int h) {
  gbuffer_ = move(unique_ptr<GBuffer>(new GBuffer()));
  gbuffer_->Init(w, h);

  LOGI("Deferred Renderer :: Geomerty Shader");
  const char *vert_file = "shader/deferred_geometry.vs";
  const char *frag_file = "shader/deferred_geometry.fs";
  unsigned int flag = 0;
  flag |= kMaterialAmbient;
  flag |= kMaterialDiffuse;
  flag |= kMaterialSpecular;
  flag |= kMaterialDiffuseMap;
  flag |= kMaterialSpecularMap;
  //normal map
  flag |= kMaterialNormalMap;

  geometry_uber_shader_.reset(new UberShader());
  geometry_uber_shader_->InitWithFile(vert_file, frag_file, flag);

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
}
void DeferredRenderer::BeginGeometryPass() {
  gbuffer_->Bind();
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  vec4ub color(0, 0, 0, 255);
  render_state.ClearBuffer(true, true, false, color);
  render_state.Set3D();

  const Material &material = render_state.LastMaterial();
  Shader &shader = geometry_uber_shader_->Load(material.props);
  render_state.UseShader(shader);

  geometry_uber_shader_->ApplyCamera();
  geometry_uber_shader_->ApplyMaterial(material);
}
void DeferredRenderer::EndGeometryPass() {
  gbuffer_->Unbind();
}
void DeferredRenderer::ApplyGeomertyPassRenderState() {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  const Material &material = render_state.LastMaterial();
  unsigned int shader_flag = material.props;
  Shader &shader = geometry_uber_shader_->Load(shader_flag);

  const mat4 &projection_mat = render_state.projection_mat();
  const mat4 &view_mat = render_state.view_mat();
  const mat4 &model_mat = render_state.model_mat();

  mat4 mvp = projection_mat * view_mat * model_mat;
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp);

  //마테리얼 정보 적절히 수동으로 설정하기
  //deferred와 forward에서의 쉐이더 코드는 완전히 다르니까 render state로 마테리얼 설정은 불가능하다
}
void DeferredRenderer::DrawMesh(Mesh *mesh) {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  const Material &material = render_state.LastMaterial();
  unsigned int shader_flag = material.props;
  Shader &shader = geometry_uber_shader_->Load(shader_flag);
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

} //namespace sora