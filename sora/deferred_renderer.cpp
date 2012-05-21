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
#include "light.h"
#include "frame_buffer.h"

#include "debug_draw_manager.h"
#include "draw_2d_manager.h"
#include "geometric_object.h"
#include "shader_manager.h"
#include "memory_file.h"
#include "filesystem.h"
#include "matrix_helper.h"
#include "post_effect.h"

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
  SR_ASSERT(gbuffer_.get() == NULL);
  SR_ASSERT(final_result_fb_.get() == NULL);

  gbuffer_ = move(unique_ptr<GBuffer>(new GBuffer()));
  gbuffer_->Init(w, h);

  LOGI("Deferred Renderer :: Geomerty Shader");
  const char *vert_file = "shader/deferred/deferred_geometry.vs";
  const char *frag_file = "shader/deferred/deferred_geometry.fs";
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


  //최종결과
  final_result_fb_.reset(new FrameBuffer());
  final_result_fb_->Init(w, h);

  return true;
}
void DeferredRenderer::Deinit() {
  if(gbuffer_.get() != NULL) {
    gbuffer_->Deinit();
    gbuffer_.reset(NULL);
  }
  if(geometry_uber_shader_ != NULL) {
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
  if(directional_shader_.get() != NULL) {
    directional_shader_->Deinit();
    directional_shader_.reset(NULL);
  }
  if(point_shader_ != NULL) {
    point_shader_->Deinit();
    point_shader_.reset(NULL);
  }
  if(normal_dump_post_effect_ != NULL) {
    normal_dump_post_effect_->Deinit();
    normal_dump_post_effect_.reset(NULL);
  }
  if(depth_dump_post_effect_ != NULL) {
    depth_dump_post_effect_->Deinit();
    depth_dump_post_effect_.reset(NULL);
  }
}

Shader &DeferredRenderer::ambient_shader() {
  if(ambient_shader_ == NULL) {
    LOGI("Deferred Renderer :: Ambient Shader");
    const char *ambient_vert_file = "shader/deferred/deferred_ambient_light.vs";
    const char *ambient_frag_file = "shader/deferred/deferred_ambient_light.fs";
    ambient_shader_.reset(new Shader());
    ambient_shader_->LoadFromFile(ambient_vert_file, ambient_frag_file);
  }
  return *ambient_shader_;  
}
Shader &DeferredRenderer::directional_shader() {
  if(directional_shader_ == NULL) {
    LOGI("Deferred Renderer :: Directional Light Shader");
    const char *define_txt = "#define DIRECTION_LIGHT 1\n";
    string vert_src = define_txt;
    string frag_src = define_txt;
    vert_src += light_vert_src();
    frag_src += light_frag_src();

    directional_shader_.reset(new Shader());
    directional_shader_->Init(vert_src, frag_src);
  }
  return *directional_shader_;
}
Shader &DeferredRenderer::depth_shader() {
  if(depth_shader_ == NULL) {
    //depth pass용으로 쓰기 위한 쉐이더
    LOGI("Deferred Renderer :: Depth Shader");
    const char *depth_vert_file = "shader/deferred/deferred_depth.vs";
    const char *depth_frag_file = "shader/deferred/deferred_depth.fs";
    depth_shader_.reset(new Shader());
    depth_shader_->LoadFromFile(depth_vert_file, depth_frag_file);
  }
  return *depth_shader_;
}

Shader &DeferredRenderer::point_shader() {
  if(point_shader_ == NULL) {
    LOGI("Deferred Renderer :: Point Light Shader");

    const char *define_txt = "#define POINT_LIGHT 1\n";
    string vert_src = define_txt;
    string frag_src = define_txt;
    vert_src += light_vert_src();
    frag_src += light_frag_src();

    point_shader_.reset(new Shader());
    point_shader_->Init(vert_src, frag_src);
  }
  return *point_shader_;
}

const std::string &DeferredRenderer::light_vert_src() {
  if(light_vert_src_.empty()) {
    const char *vert_file = "shader/deferred/deferred_light.vs";
    string vert_path = Filesystem::GetAppPath(vert_file);
    MemoryFile mem_file(vert_path);
    bool opened = mem_file.Open();
    SR_ASSERT(opened == true);
    const char *src = (const char*)(mem_file.start);
    light_vert_src_ = src;
    mem_file.Close();
  }
  return light_vert_src_;
}

const std::string &DeferredRenderer::light_frag_src() {
  if(light_frag_src_.empty()) {
    const char *frag_file = "shader/deferred/deferred_light.fs";
    string frag_path = Filesystem::GetAppPath(frag_file);
    MemoryFile mem_file(frag_path);
    bool opened = mem_file.Open();
    SR_ASSERT(opened == true);
    const char *src = (const char*)(mem_file.start);

    const std::string &light_model_src = Light::GetLightModelFragSrc();
    light_frag_src_ = light_model_src;
    light_frag_src_ += src;

    mem_file.Close();
  }
  return light_frag_src_;
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
  render_state.UseShader(depth_shader());
}
void DeferredRenderer::DrawDepthPass(Mesh *mesh) {
  Device *dev = Device::GetInstance();
  RenderState &render_state = dev->render_state();
  //mesh에 따라서 model 행렬은 변하니까 begin같은곳에 떄려박지 못하고
  //메시마다 따로 대입한다
  mat4 mvp = render_state.GetMVPMatrix();
  depth_shader().SetUniformMatrix(kMVPHandleName, mvp);
  depth_shader().DrawMeshIgnoreMaterial(mesh);
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

  //normal vector를 적절히 돌리기
  const mat4 &view_mat = render_state.view_mat();
  const mat4 &model_mat = render_state.model_mat();
  mat4 modelview_mat4 = view_mat * model_mat;
  mat4 modelview_mat4_inv = glm::inverse(modelview_mat4);
  mat3 modelview_inv(modelview_mat4_inv);
  mat3 modelview_inv_transpose = glm::transpose(modelview_inv);
  const char *rotation_mat_var_name = "u_rotationMat";
  shader.SetUniformMatrix(rotation_mat_var_name, modelview_inv_transpose);


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

void DeferredRenderer::BeginLightPass() {
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  render_state.Set2D();

  //일반 2d blend와 다르게 지속적으로 색을 누적시킨다
  glBlendFunc(GL_ONE, GL_ONE);

  final_result_fb_->Bind();

  vec4ub color(0, 0, 0, 255);
  render_state.ClearBuffer(true, true, false, color);

  //blend잘 써서 같은 버퍼에 덮어서 그린다고 이전 색깔 정보 날라가지 않고
  //적절히 누적되도록함
}
void DeferredRenderer::EndLightPass() {
  final_result_fb_->Unbind();
  //원래 2d스타일의 bleud로 되돌리기
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
  //동적 라이팅중에서 구조가 가장 간단한 방향으로 뱉어지는 빛이다. 이것의 경우
  //그냥 화면 전체에 대해서 2d로 잘 적용하면 된다
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  Shader &shader = directional_shader();
  render_state.UseShader(shader);

  //빛 계산에 쓰이는 공용 속성 설정
  SetCommonLightUniform(shader, light);

  //방향성빛은 방향만 잇으니까 행렬의 3*3만 쓴다
  mat3 model_mat(render_state.model_mat());
  mat3 light_mat(model_mat);

  vec3 light_target_pos(light.dir);
  vec3 viewspace_light_target_pos(light_mat * light_target_pos);
  vec3 light_dir = normalize(viewspace_light_target_pos);

  //빛 방향을 디버깅을 위해서 출력하기
  Draw2DManager *draw_2d_mgr = device->draw_2d();
  char light_dir_buf[128];  
  sprintf(light_dir_buf, "LightDir:%.4f, %.4f, %.4f", light_dir.x, light_dir.y, light_dir.z);
  draw_2d_mgr->AddString(vec2(0, 100), light_dir_buf, Color4ub::Red(), 1.0f);
  shader.SetUniformVector("u_lightDir", light_dir);

  SetCommonLightQuadDraw(shader);
}
void DeferredRenderer::DrawPointLight(const Light &light) {
  SR_ASSERT(light.type == kLightPoint);

  Device *dev = Device::GetInstance();
  RenderState &render_state = dev->render_state();

  //해당위치의 스텐실 버퍼에 원을 그린다. 원으로 빛이 영향을 줄 영역을 결정해서
  //depth test를 오래하지 않도록한다
  //스텐실 버퍼에 의존하는 방식보다 scissor로 고치는게 성능상 유리할테니
  //나중에는 적절히 고치자
  GeometricObject<vec3> sphere_mesh;
  sphere_mesh.SolidSphere(1, 16, 16);

  const bool use_stencil = true;
  if(use_stencil) {
    //3d장면에 렌더링하기. 그래야 빛이 영향줄 구가 제대로 그려진다
    render_state.Set3D();
    const mat4 &projection = render_state.projection_mat();
    const mat4 &view = render_state.view_mat();
    mat4 model(1.0f);
    model = glm::translate(model, light.pos);
    model = glm::scale(model, vec3(light.radius));
    mat4 mvp_3d = projection * view * model;

    //단색쉐이더로 대충 그리기. 스텐실 버퍼에만 그려지면 됨
    //색칠 영역은 스텐실 버퍼니까 적절히 초기화하기
    render_state.ClearBuffer(false, false, true, Color4ub::White());
    glColorMask(false, false, false, false);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 0xff, 0xff);

    ShaderManager *shader_mgr = dev->shader_mgr();
    Shader *const_color_shader = shader_mgr->Get(ShaderManager::kConstColor);
    render_state.UseShader(*const_color_shader);
    vec4 white(1.0f);
    const_color_shader->SetUniformVector(kConstColorHandleName, white);
  
    const_color_shader->SetUniformMatrix(kMVPHandleName, mvp_3d);
    const DrawCmdData<vec3> &draw_cmd = sphere_mesh.cmd_list().at(0);
    const_color_shader->SetVertexList(draw_cmd.vertex_list);
    const_color_shader->DrawElements(draw_cmd.draw_mode, draw_cmd.index_list);
  }
  {
    //스텐실 영역에 잇는거 진짜로 그리기. 2D로 그리기
    //이것을 진행하면서 빛 계산을 수행한다
    if(use_stencil) {
      glColorMask(true, true, true, true);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      glStencilFunc(GL_EQUAL, 0xff, 0xff);
    }

    //점광원 계산용 쉐이더로 교체
    Shader &point_shader = this->point_shader();
    render_state.UseShader(point_shader);
    //빛 계산에 쓰이는 공용 속성 설정
    SetCommonLightUniform(point_shader, light);

    const mat4 &view = render_state.view_mat();
    const mat4 &model = render_state.model_mat();
    vec4 light_pos = model * vec4(light.pos, 1.0f);
    light_pos.w = light.radius; //4번쨰를 반지름으로 사용

    {
      Draw2DManager *draw_2d_mgr = dev->draw_2d();
      char light_pos_buf[128];  
      sprintf(light_pos_buf, "LightPos:%.4f, %.4f, %.4f", light_pos.x, light_pos.y, light_pos.z);
      draw_2d_mgr->AddString(vec2(0, 100), light_pos_buf, Color4ub::Green(), 1.0f);

      DebugDrawManager *draw_3d_mgr = dev->debug_draw_mgr();
      draw_3d_mgr->AddString(light.pos, "Light", Color4ub::Red(), 1.0f);
    }

    point_shader.SetUniformVector("u_lightPos", light_pos);
    

    //2d로 교체는 렌더링 직전에 수행하자
    render_state.Set2D();

    SetCommonLightQuadDraw(point_shader);
  }
  
  //restore state
  if(use_stencil) {
    glDisable(GL_STENCIL_TEST);
  }
}

void DeferredRenderer::SetCommonLightUniform(Shader &shader, const Light &light) {
  Device *dev = Device::GetInstance();
  RenderState &render_state = dev->render_state();

  ShaderVariable diffuse_var = shader.uniform_var(kDiffuseMapHandleName);
    ShaderVariable specular_var = shader.uniform_var(kSpecularMapHandleName);
    ShaderVariable depth_var = shader.uniform_var("s_depth");
    ShaderVariable normal_var = shader.uniform_var("s_viewSpaceNormal");
    ShaderVariable pos_var = shader.uniform_var("s_positionMap");

    struct ShaderVarTexParam {
      ShaderVarTexParam(const ShaderVariable &shader_var, const Texture &tex)
        : shader_var(shader_var), tex(tex) {}
      ShaderVariable shader_var;
      Texture tex;
    };
    vector<ShaderVarTexParam> var_tex_param_list;
    var_tex_param_list.push_back(ShaderVarTexParam(diffuse_var, DiffuseTex()));
    var_tex_param_list.push_back(ShaderVarTexParam(specular_var, SpecularTex()));
    var_tex_param_list.push_back(ShaderVarTexParam(depth_var, DepthTex()));
    var_tex_param_list.push_back(ShaderVarTexParam(normal_var, NormalTex()));
    var_tex_param_list.push_back(ShaderVarTexParam(pos_var, PositionTex()));
    int tex_unit = 0;
    for(size_t i = 0 ; i < var_tex_param_list.size() ; i++) {
      ShaderVarTexParam &param = var_tex_param_list[i];
      if(param.shader_var.location != kInvalidShaderVarLocation) {
        render_state.UseTexture(param.tex, tex_unit);
        SetUniformValue(param.shader_var, tex_unit);
        tex_unit++;
      }
    }

  shader.SetUniformVector(kClipPlaneHandleName, render_state.GetClipPlanes3D());
  shader.SetUniformMatrix(kMVPHandleName, mat4(1.0f));

  const mat4 &projection = render_state.projection_mat();
  const mat4 &view = render_state.view_mat();
  const mat4 &model = render_state.model_mat();
  mat4 mvp = projection * view * model;
  mat4 mvp_inv = glm::inverse(mvp);
  shader.SetUniformMatrix(kMVPInverseHandleName, mvp_inv);
  shader.SetUniformMatrix(kViewHandleName, view);

  mat4 projection_inv = glm::inverse(projection);
  shader.SetUniformMatrix(kProjectionInvHandleName, projection_inv);

  mat4 mv = view * model;
  shader.SetUniformMatrix(kModelViewHandleName, mv);

  vec4 viewport(0, 0, render_state.win_width(), render_state.win_height());
  shader.SetUniformVector(kViewportHandleName, viewport);

  shader.SetUniformVector(kDiffuseColorHandleName, light.diffuse);
  shader.SetUniformVector(kSpecularColorHandleName, light.specular);
}
void DeferredRenderer::SetCommonLightQuadDraw(Shader &shader) {
  Device *dev = Device::GetInstance();
  RenderState &render_state = dev->render_state();

  const mat4 &projection_mat = render_state.GetProjection3D();
  const mat4 &view_mat = render_state.view_mat();
  const mat4 &model_mat = render_state.model_mat();
  mat4 modelview = view_mat * model_mat;
  //벡터방향만 잇으면 되니까 이동관련속성은 날리기
  for(int i = 0 ; i < 3 ; ++i) {
    modelview[3][i] = 0;
  }
  vec4 viewport(0, 0, render_state.win_width(), render_state.win_height());

  float pixels[4][2] = {
    { 0, 0 },
    { viewport[2], 0 },
    { viewport[2], viewport[3] },
    { 0, viewport[3] },
  };
  vec3 view_vec_list[4];
  for(int i = 0 ; i < 4 ; i++) {
    vec3 win_coord(pixels[i][0], pixels[i][1], 10);
    vec3 obj = glm::unProject(win_coord, modelview, projection_mat, viewport);
    vec3 &view_vec = view_vec_list[i];
    view_vec = obj;
    vec3 cam_pos = MatrixHelper::ViewPos(view_mat);
    view_vec -= cam_pos;
    view_vec = glm::normalize(view_vec);

    mat3 modelview_mat3(modelview);
    view_vec = modelview_mat3 * view_vec;
  }

  //fragment마다 view vector를 계산하기 위해서 계산에 필요한 정보까지 넣기
  //4군데만 넣어주면 나머지는 알아서 적절히 보간될것이다
  struct LightQuadVertex {
    LightQuadVertex(float x, float y, float s, float t, const vec3 &view_vec)
      : pos(x, y), texcoord(s, t), view_vec(view_vec) {}
    vec2 pos;
    vec2 texcoord;
    vec3 view_vec;
  };
  vector<LightQuadVertex> vert_list;
  vert_list.push_back(LightQuadVertex(-1, -1, 0, 0, view_vec_list[0]));
  vert_list.push_back(LightQuadVertex(1, -1, 1, 0, view_vec_list[1]));
  vert_list.push_back(LightQuadVertex(1, 1, 1, 1, view_vec_list[2]));
  vert_list.push_back(LightQuadVertex(-1, 1, 0, 1, view_vec_list[3]));

  ShaderVariable pos_var = shader.attrib_var(kPositionHandleName);
  ShaderVariable texcoord_var = shader.attrib_var(kTexcoordHandleName);
  ShaderVariable view_vec_var = shader.attrib_var("a_viewVector");
  
  {
    SR_ASSERT(pos_var.location != kInvalidShaderVarLocation);
    AttribBindParam param;
    param.dim = 2;
    param.normalize = false;
    param.offset = offsetof(LightQuadVertex, pos);
    param.var_type = GL_FLOAT;
    param.vert_size = sizeof(LightQuadVertex);
    void *ptr = &vert_list[0];
    SetAttrib(pos_var, param, (char*)ptr);
  }
  {
    SR_ASSERT(texcoord_var.location != kInvalidShaderVarLocation);
    AttribBindParam param;
    param.dim = 2;
    param.normalize = false;
    param.offset = offsetof(LightQuadVertex, texcoord);
    param.var_type = GL_FLOAT;
    param.vert_size = sizeof(LightQuadVertex);
    void *ptr = &vert_list[0];
    SetAttrib(texcoord_var, param, (char*)ptr);
  }
  if(view_vec_var.location != kInvalidShaderVarLocation) {
    AttribBindParam param;
    param.dim = 3;
    param.normalize = true;
    param.offset = offsetof(LightQuadVertex, view_vec);
    param.var_type = GL_FLOAT;
    param.vert_size = sizeof(LightQuadVertex);
    void *ptr = &vert_list[0];
    SetAttrib(view_vec_var, param, (char*)ptr);
  }

  shader.DrawArrays(kDrawTriangleFan, vert_list.size());

  /*
  vector<Vertex2D> vert_list;
  vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
  vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
  vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));
  shader.DrawArrays(kDrawTriangleFan, vert_list);
  */
}
void DeferredRenderer::DrawSpotLight(const Light &light) {
  SR_ASSERT(light.type == kLightSpotLight);
}
void DeferredRenderer::DrawAmbientLight(const glm::vec3 &color) {
  //화면 전체를 단색으로 그리기
  Device *device = Device::GetInstance();
  RenderState &render_state = device->render_state();
  Shader &shader = ambient_shader();
  render_state.UseShader(shader);

  mat4 mvp(1.0f);
  shader.SetUniformMatrix(kMVPHandleName, mvp);

  vec4 ambient_color(color, 1.0f);
  shader.SetUniformVector(kAmbientColorHandleName, ambient_color);

  Texture diffuse_tex = DiffuseTex();
  render_state.UseTexture(diffuse_tex, 0);
  
  vector<Vertex2D> vert_list;
  vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
  vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
  vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));
  shader.DrawArrays(kDrawTriangleFan, vert_list);
}
unsigned int DeferredRenderer::GBufferHandle() const {
  return gbuffer_->fbo();
}
Texture &DeferredRenderer::FinalResultTex() const {
  return final_result_fb_->color_tex();
}
Texture DeferredRenderer::PositionTex() const {
  return gbuffer_->PositionTex();
}
void DeferredRenderer::DrawPointLightArea(const Light &light) {
  Device *dev = Device::GetInstance();
  const RenderState &render_state = dev->render_state();
  DebugDrawManager *draw_mgr = dev->debug_draw_mgr();

  float radius = light.radius;
  const vec3 &pos = light.pos;
  
  draw_mgr->AddSphere(pos, radius, Color4ub::White());
}
PostEffect &DeferredRenderer::NormalDumpPostEffect() {
  if(normal_dump_post_effect_ == NULL) {
    normal_dump_post_effect_.reset(new PostEffect());
    string fs_path = Filesystem::GetAppPath("shader/deferred/normal_visualization.fs");
    normal_dump_post_effect_->InitFromFile(fs_path);
  }
  return *normal_dump_post_effect_;
}
PostEffect &DeferredRenderer::DepthDumpPostEffect() {
  if(depth_dump_post_effect_ == NULL) {
    depth_dump_post_effect_.reset(new PostEffect());
    string fs_path = Filesystem::GetAppPath("shader/deferred/depth_visualization.fs");
    depth_dump_post_effect_->InitFromFile(fs_path);
  }
  return *depth_dump_post_effect_;
}
void DeferredRenderer::DumpNormalTex() {
  Texture tex = NormalTex();
  NormalDumpPostEffect().Draw(tex);
}
void DeferredRenderer::DumpDepthTex() {
  Texture tex = DepthTex();
  PostEffect &post_effect = DepthDumpPostEffect();
  post_effect.BeginPass();

  vec2 clip_plane = Device::GetInstance()->render_state().GetClipPlanes3D();
  Shader &shader = post_effect.post_effect();
  shader.SetUniformVector(kClipPlaneHandleName, clip_plane);
  post_effect.LowLevelDraw(tex);
  post_effect.EndPass();
}
} //namespace sora