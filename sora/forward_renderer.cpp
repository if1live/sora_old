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
#include "forward_renderer.h"
#include "render_state.h"
#include "device.h"
#include "mesh.h"
#include "uber_shader.h"
#include "light.h"
#include "texture.h"

using namespace glm;
using namespace std;

namespace sora {;
ForwardRenderer::ForwardRenderer() {
}
ForwardRenderer::~ForwardRenderer() {
  Deinit();
}

void ForwardRenderer::BeginPass() {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();

  vec4ub color(128, 0, 0, 255);
  render_state.ClearBuffer(true, true, false, color);
  //3d
  render_state.Set3D();
}
void ForwardRenderer::EndPass() {
}
void ForwardRenderer::ApplyRenderState() {
  Device *device = Device::GetInstance();
  const Material &mtl = device->render_state().LastMaterial();
  Shader &shader = uber_shader_->Load(mtl.props);
  
  device->render_state().UseShader(shader);
  //TODO 광원은 render state로 넘기기
  uber_shader_->ApplyCamera();  //설정된 쉐이더 + 광원 + 카메라 정보를 uber shader로 넘기기
  const vec4 &light_ambient = light_->ambient;
  const vec4 &light_diffuse = light_->diffuse;
  const vec4 &light_specular = light_->specular;
  uber_shader_->ApplyMaterial(light_ambient, light_diffuse, light_specular);
}
void ForwardRenderer::DrawMesh(Mesh *mesh) {
  Device *device = Device::GetInstance();
  const Material &mtl = device->render_state().LastMaterial();
  Shader &shader = uber_shader_->Load(mtl.props);
  shader.DrawMeshIgnoreMaterial(mesh);
}

void ForwardRenderer::SetLight(const Light &light) {
  *light_ = light;
}
void ForwardRenderer::Init() {
  SR_ASSERT(uber_shader_.get() == NULL);
  uber_shader_ = move(unique_ptr<UberShader>(new UberShader()));

  const char *vert_file = "shader/per_pixel_uber.vs";
  const char *frag_file = "shader/per_pixel_uber.fs";
  unsigned int flag = 0;
  flag |= kMaterialAmbient;
  flag |= kMaterialDiffuse;
  flag |= kMaterialSpecular;
  flag |= kMaterialDiffuseMap;
  flag |= kMaterialSpecularMap;
  //normal map
  flag |= kMaterialNormalMap;
  uber_shader_->InitWithFile(vert_file, frag_file, flag);

  light_.reset(new Light());
}
void ForwardRenderer::Deinit() {
  if(uber_shader_.get() != NULL) {
    uber_shader_->Deinit();
    uber_shader_.reset(NULL);
  }
}
} //namespace sora