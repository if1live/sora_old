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
#include "gl_uber_shader_renderer.h"

#include "render_device.h"
#include "uber_shader.h"
#include "texture.h"
#include "camera.h"
#include "device.h"

using namespace std;
using namespace glm;

namespace sora {;
namespace gl {
  GLUberShaderRenderer::GLUberShaderRenderer() {
    material_ = Material::NullMaterial();
  }
  GLUberShaderRenderer::~GLUberShaderRenderer() {
  }
  void GLUberShaderRenderer::Init() {
    uber_shader_.Init<LightUberShaderLoadPolicy>();
  }
  Shader &GLUberShaderRenderer::GetCurrShader() {
    unsigned int flag = material_.props;
    Shader &shader = uber_shader_.Load(flag);
    return shader;
  }
  void GLUberShaderRenderer::SetCamera(const Camera &cam) {
    glm::mat4 model(1.0f);
    SetCamera(cam, model);
  }
  void GLUberShaderRenderer::SetCamera(const Camera &cam, const glm::mat4 &model) {
    Device *dev = Device::GetInstance();
    RenderDevice *render_dev = &dev->render_device();

    float win_w = (float)render_dev->win_width();
    float win_h = (float)render_dev->win_height();
    glm::mat4 projection = glm::perspective(45.0f, win_w/ win_h, 0.1f, 100.0f);
    glm::mat4 view = cam.LookAt();

    Shader &shader = GetCurrShader();

    //world-view-projection
    //world, view, projection 같은것을 등록할수 잇으면 등록하기
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= projection;
    mvp *= view;
    mvp *= model;
    shader.SetUniformMatrix(kMVPHandleName, mvp);
    shader.SetUniformMatrix(kModelHandleName, model);
    shader.SetUniformMatrix(kProjectionHandleName, projection);
    shader.SetUniformMatrix(kViewHandleName, view);

    const glm::vec3 &eye = cam.eye;
    const glm::vec3 &center = cam.center;
    const glm::vec3 &up = cam.up;
    glm::vec3 dir = center - eye;
    glm::vec3 view_side = glm::cross(dir, up);

    glm::vec4 view_side_vec(view_side.x, view_side.y, view_side.z, 1.0f);
    shader.SetUniformVector(kViewSideHandleName, view_side);

    glm::vec4 eye_vec(eye.x, eye.y, eye.z, 1.0f);
    shader.SetUniformVector(kViewPositionHandleName, eye_vec);

    glm::vec4 up_vec(up.x, up.y, up.z, 1.0f);
    shader.SetUniformVector(kViewUpHandleName, up_vec);

    glm::vec4 dir_vec(dir.x, dir.y, dir.z, 1.0f);
    shader.SetUniformVector(kViewDirHandleName, dir_vec);
  }

  void GLUberShaderRenderer::ApplyMaterialLight() {
    Device *dev = Device::GetInstance();
    RenderDevice *render_dev = &dev->render_device();

    unsigned int flag = material_.props;
    Shader &shader = GetCurrShader();
    const Material &material = material_;
    const Light &light = light_;
    
    //apply light pos
    shader.SetUniformVector(kLightPositionHandleName, light_.pos);  //3?4?
    SR_CHECK_ERROR("Set Light Pos Handle");

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
        render_dev->UseTexture(*diffuse_map);
        shader.SetUniformValue(kDiffuseMapHandleName, 0);
        SR_CHECK_ERROR("Set Ambient map");
      }
    }
    if(use_specular_map) {
      Texture *specular_map = dev->tex_mgr()->Get_ptr(material.specular_map);
      if(specular_map != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_map->handle());
        shader.SetUniformValue(kSpecularMapHandleName, 1);
        SR_CHECK_ERROR("Set Specular map");
      }
    }
    if(use_normal_map) {
      Texture *normal_map = dev->tex_mgr()->Get_ptr(material.normal_map);
      if(normal_map != NULL) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normal_map->handle());
        shader.SetUniformValue(kNormalMapHandleName, 2);
        SR_CHECK_ERROR("Set Normal map");
      }
    }
    //최초 상태로 돌려놓기
    glActiveTexture(GL_TEXTURE0);
    SR_CHECK_ERROR("Apply Material & Light");
  }
  void GLUberShaderRenderer::SetMaterial(const Material &mtl) {
    material_ = mtl;
  }
  void GLUberShaderRenderer::SetLight(const Light &light) {
    light_ = light;
  }
} //namespace gl
} //namespace sora