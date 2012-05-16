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

using namespace std;
using namespace glm;

namespace sora {;

DeferredRenderer::DeferredRenderer() {
}
DeferredRenderer::~DeferredRenderer() {
  Deinit();
}
bool DeferredRenderer::Init(int w, int h) {
  material_.reset(new Material());
  gbuffer_ = move(unique_ptr<GBuffer>(new GBuffer()));
  gbuffer_->Init(w, h);

  LOGI("Deferred Renderer :: Geomerty Shader");
  string deferred_geomerty_vs_path = Filesystem::GetAppPath("shader/deferred_geometry.vs");
  string deferred_geomerty_fs_path = Filesystem::GetAppPath("shader/deferred_geometry.fs");
  geometry_shader_ = move(unique_ptr<Shader>(new Shader()));
  geometry_shader_->LoadFromFile(deferred_geomerty_vs_path, deferred_geomerty_fs_path);
  return true;
}
void DeferredRenderer::Deinit() {
  if(gbuffer_.get() != NULL) {
    gbuffer_->Deinit();
  }
  if(geometry_shader_.get() != NULL) {
    geometry_shader_->Deinit();
  }
}
void DeferredRenderer::BeginGeometryPass() {
  gbuffer_->Bind();
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  vec4ub color(0, 0, 0, 255);
  render_state.ClearBuffer(true, true, false, color);
  render_state.Set3D();
}
void DeferredRenderer::EndGeometryPass() {
  gbuffer_->Unbind();
}
void DeferredRenderer::ApplyGeomertyPassRenderState() {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();

  render_state.UseShader(*geometry_shader_);

  const mat4 &projection_mat = render_state.projection_mat();
  const mat4 &view_mat = render_state.view_mat();
  const mat4 &model_mat = render_state.model_mat();

  mat4 mvp = projection_mat * view_mat * model_mat;
  ShaderVariable mvp_var = geometry_shader_->uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp);

  mat3 view_mat3(view_mat);
  mat3 view_mat3_inv = glm::inverse(view_mat3);
  mat3 view_mat3_inv_transpose = glm::transpose(view_mat3_inv);
  ShaderVariable view_inv_transpose_var = geometry_shader_->uniform_var("u_model3_InverseTranspose");
  SR_ASSERT(view_inv_transpose_var.location != kInvalidShaderVarLocation);
  SetUniformMatrix(view_inv_transpose_var, view_mat3_inv_transpose);

  //마테리얼 정보 적절히 수동으로 설정하기
  //deferred와 forward에서의 쉐이더 코드는 완전히 다르니까 render state로 마테리얼 설정은 불가능하다
}
void DeferredRenderer::DrawMesh(Mesh *mesh) {
  geometry_shader_->DrawMeshIgnoreMaterial(mesh);
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

void DeferredRenderer::SetMaterial(const Material &mtl) {
  *material_ = mtl;
}

} //namespace sora