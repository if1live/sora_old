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
  ApplyMaterialWithLight();
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
  uber_shader_->Init<LightUberShaderLoadPolicy>();

  light_.reset(new Light());
}
void ForwardRenderer::Deinit() {
  if(uber_shader_.get() != NULL) {
    uber_shader_->Deinit();
    uber_shader_.reset(NULL);
  }
}
void ForwardRenderer::ApplyMaterialWithLight() {
  Device *dev = Device::GetInstance();
  RenderState *render_dev = &dev->render_state();
  const Material &mtl = dev->render_state().LastMaterial();
  Shader &shader = uber_shader_->Load(mtl.props);
  const Material &material = render_dev->LastMaterial();
  const Light &light = *light_;

  //apply light pos
  shader.SetUniformVector(kLightPositionHandleName, light_->pos);  //3?4?
  SR_CHECK_ERROR("Set Light Pos Handle");

  unsigned int flag = material.props;
  bool use_ambient = ((flag & kMaterialAmbient) == kMaterialAmbient);
  bool use_diffuse = ((flag & kMaterialDiffuse) == kMaterialDiffuse);
  bool use_specular = ((flag & kMaterialSpecular) == kMaterialSpecular);
  bool use_diffuse_map = ((flag & kMaterialDiffuseMap) == kMaterialDiffuseMap);
  bool use_specular_map = ((flag & kMaterialSpecularMap) == kMaterialSpecularMap);
  bool use_normal_map = ((flag & kMaterialNormalMap) == kMaterialNormalMap);

  vec4 ambient_color;
  vec4 diffuse_color;
  vec4 specular_color;

  //색 정보 얻기
  if(use_ambient) {
    //material의 색속성
    ambient_color = material.ambient;
    //빛속성과 조합
    for(int i = 0 ; i < 4 ; ++i) {
      ambient_color[i] *= light.ambient[i];
    }
    shader.SetUniformVector(kAmbientColorHandleName, ambient_color);
    SR_CHECK_ERROR("Set Ambient Color");
  }
  if(use_diffuse) {
    diffuse_color = material.diffuse;
    for(int i = 0 ; i < 4 ; ++i) {
      diffuse_color[i] *= light.diffuse[i];
    }
    shader.SetUniformVector(kDiffuseColorHandleName, diffuse_color);
    SR_CHECK_ERROR("Set Diffuse Color");
  }
  if(use_specular) {
    specular_color = material.specular;
    for(int i = 0 ; i < 4 ; ++i) {
      specular_color[i] *= light.specular[i];
    }
    shader.SetUniformVector(kSpecularColorHandleName, specular_color);
    SR_CHECK_ERROR("Set Specular Color");
    shader.SetUniformValue(kSpecularShininessHandleName, material.shininess);
    SR_CHECK_ERROR("Set Shininess Color");
  }
  //마지막에 등록된 재질과 지금 처리중인 재질이 다른 경우에만 아래의 텍스쳐 바인딩을 처리하자
  if(use_diffuse_map) {
    Texture *diffuse_map = dev->tex_mgr()->Get_ptr(material.diffuse_map);
    if(diffuse_map != NULL) {
      render_dev->UseTexture(*diffuse_map, 0);
      shader.SetUniformValue(kDiffuseMapHandleName, 0);
      SR_CHECK_ERROR("Set Ambient map");
    }
  }
  if(use_specular_map) {
    Texture *specular_map = dev->tex_mgr()->Get_ptr(material.specular_map);
    if(specular_map != NULL) {
      render_dev->UseTexture(*specular_map, 1);
      shader.SetUniformValue(kSpecularMapHandleName, 1);
      SR_CHECK_ERROR("Set Specular map");
    }
  }
  if(use_normal_map) {
    Texture *normal_map = dev->tex_mgr()->Get_ptr(material.normal_map);
    if(normal_map != NULL) {
      render_dev->UseTexture(*normal_map, 2);
      shader.SetUniformValue(kNormalMapHandleName, 2);
      SR_CHECK_ERROR("Set Normal map");
    }
  }
  //최초 상태로 돌려놓기
  glActiveTexture(GL_TEXTURE0);
  SR_CHECK_ERROR("Apply Material & Light");
}
} //namespace sora