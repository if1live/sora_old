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
#include "uber_shader.h"
#include "shader.h"
#include "filesystem.h"
#include "memory_file.h"
#include "device.h"
#include "render_state.h"
#include "matrix_helper.h"
#include "texture.h"

using namespace std;

namespace sora {;

//uber shader의 enum에 정의된 순서대로 맞춰서 쓴다
//define만이 아니라 숫자까지 넣어준이유는 쉐이더 코드에서 바로 갖다쓰기 위해서
const char *enable_define_list[] = {
  "#define AMBIENT_MASK 1 \n",
  "#define DIFFUSE_MASK 1 \n",
  "#define SPECULAR_MASK 1 \n",
  "#define DIFFUSE_MAP_MASK 1 \n",
  "#define SPECULAR_MAP_MASK 1 \n",
  "#define NORMAL_MAP_MASK 1 \n",
};
//undef가 아니라 0으로 해서 쉐이더코드쪽에서 바로 갖다쓸수 잇도록햇다
const char *disable_define_list[] = {
  "#define AMBIENT_MASK 0 \n",
  "#define DIFFUSE_MASK 0 \n",
  "#define SPECULAR_MASK 0 \n",
  "#define DIFFUSE_MAP_MASK 0 \n",
  "#define SPECULAR_MAP_MASK 0 \n",
  "#define NORMAL_MAP_MASK 0 \n",
};


UberShader::UberShader() : avail_mask_(0) {
}

void UberShader::LoadRawSrc(const std::string &v_file, const std::string &f_file) {
  //uber shader 로딩
  std::string app_vert_path = Filesystem::GetAppPath(v_file);
  std::string app_frag_path = Filesystem::GetAppPath(f_file);
  MemoryFile vert_file(app_vert_path);
  MemoryFile frag_file(app_frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  orig_vert_src_ = vert_src;
  orig_frag_src_ = frag_src;
  vert_file.Close();
  frag_file.Close();
}

UberShader::~UberShader() {
  Deinit();
}
void UberShader::Deinit() {
  auto it = prog_dict_.begin();
  auto endit = prog_dict_.end();
  for( ; it != endit ; ++it) {
    it->second.Deinit();
  }
  prog_dict_.clear();
}

bool UberShader::IsValidFlag(uint flag) const {
  if((~avail_mask_ & flag) != 0) {
    return false;
  } else {
    return true;
  }
}

Shader &UberShader::Load(uint flag) {
  SR_ASSERT(orig_vert_src_.empty() == false && "not initialized yet");
  SR_ASSERT(orig_frag_src_.empty() == false && "not initialized yet");

  //shader마다 허용 가능한 플래그가 약간 다르다
  //요청한것에 대해서 미리 검사하기
  if((~avail_mask_ & flag) != 0) {
    //LOGW("사용할수 없는 flag를 활성화 햇음. 자동 무시");
    flag = flag & avail_mask_;
  }

  auto found = prog_dict_.find(flag);
  if(found != prog_dict_.end()) {
    return found->second;
  }
  //없으면 적절히 생성하기
  vector<const char*> vert_src_list;
  vector<const char*> frag_src_list;

  const int enable_count = sizeof(enable_define_list) / sizeof(enable_define_list[0]);
  const int disable_count = sizeof(disable_define_list) / sizeof(disable_define_list[0]);
  SR_STATIC_ASSERT(enable_count == disable_count, "must same size");
  for(int i = 0 ; i < enable_count ; i++) {
    unsigned int mask = 1 << i;
    if((flag & mask) == mask) {
      vert_src_list.push_back(enable_define_list[i]);
      frag_src_list.push_back(enable_define_list[i]);
    } else {
      vert_src_list.push_back(disable_define_list[i]);
      frag_src_list.push_back(disable_define_list[i]);
    }
  }
  vert_src_list.push_back(orig_vert_src_.c_str());
  frag_src_list.push_back(orig_frag_src_.c_str());


  //쉐이더 프로그램 적절히 생성
  Shader shader_prog;
  shader_prog.Init(vert_src_list, frag_src_list);

  prog_dict_[flag] = shader_prog;
  return prog_dict_[flag];
}

void UberShader::ApplyMaterial() {
  Device *dev = Device::GetInstance();
  RenderState *render_dev = &dev->render_state();
  const Material &material = render_dev->LastMaterial();
  unsigned int flag = material.props;

  Shader &shader = Load(flag);
  
  bool use_ambient = ((flag & kMaterialAmbient) == kMaterialAmbient);
  bool use_diffuse = ((flag & kMaterialDiffuse) == kMaterialDiffuse);
  bool use_specular = ((flag & kMaterialSpecular) == kMaterialSpecular);
  bool use_diffuse_map = ((flag & kMaterialDiffuseMap) == kMaterialDiffuseMap);
  bool use_specular_map = ((flag & kMaterialSpecularMap) == kMaterialSpecularMap);
  bool use_normal_map = ((flag & kMaterialNormalMap) == kMaterialNormalMap);

  glm::vec4 ambient_color;
  glm::vec4 diffuse_color;
  glm::vec4 specular_color;

  //색 정보 얻기
  if(use_ambient) {
    //material의 색속성
    ambient_color = material.ambient;
    shader.SetUniformVector(kAmbientColorHandleName, ambient_color);
    SR_CHECK_ERROR("Set Ambient Color");
  }
  if(use_diffuse) {
    diffuse_color = material.diffuse;
    shader.SetUniformVector(kDiffuseColorHandleName, diffuse_color);
    SR_CHECK_ERROR("Set Diffuse Color");
  }
  if(use_specular) {
    specular_color = material.specular;
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
  SR_CHECK_ERROR("Apply Material");
}

void UberShader::ApplyCamera() {
  Device *dev = Device::GetInstance();
  RenderState *render_dev = &dev->render_state();
  const glm::mat4 &projection = render_dev->projection_mat();
  const glm::mat4 &view = render_dev->view_mat();
  const glm::mat4 &model = render_dev->model_mat();
  const Material &material = render_dev->LastMaterial();

  Shader &shader = Load(material.props);

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

  //빛 계산에는 normal계산 뒤집는 행렬이 필요하다
  glm::mat4 model_mat4(model);
  glm::mat4 model_mat4_inv = glm::inverse(model_mat4);
  glm::mat4 model_mat4_inv_transpose = glm::transpose(model_mat4_inv);
  shader.SetUniformMatrix(kModelInverseTransposeHandleName, model_mat4_inv_transpose);

  const glm::vec3 eye = MatrixHelper::ViewPos(view);
  const glm::vec3 up = MatrixHelper::ViewUpVec(view);
  const glm::vec3 dir = MatrixHelper::ViewDirVec(view);
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

} //namespace sora


