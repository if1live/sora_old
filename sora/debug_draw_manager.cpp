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
#include "debug_draw_manager.h"
#include "template_lib.h"
#include "shader.h"
#include "render_state.h"
#include "geometric_object.h"
#include "matrix_stack.h"
#include "device.h"
#include "render_state.h"

#include "draw_2d_manager.h"
#include "shader_manager.h"
#include "sys_font.h"

using namespace std;
using namespace glm;

namespace sora {;

//디버깅 렌더링에서 쓰이는 쉐이더는 크게 2가지이다
//쉐이더 고정 단색 + 좌표정보만 잇는 버텍스를 처리할수 잇는 쉐이더. 이것은 선이나 뭐 그런거에 쓴다
//텍스쳐 + 쉐이더 고정 단색 + 좌표정보 잇는 버텍스. 문자출력에 사용한다

struct DebugDrawCmd {
  typedef enum {
    kDebugDrawNone,
    kDebugDrawLine,
    kDebugDrawCross,
    kDebugDrawSphere,
    kDebugDrawAxis,
    kDebugDrawString
  } DebugDrawType;

  DebugDrawCmd(DebugDrawType type)
    : type(type),
    duration(0),
    depth_enable(true) {
    RenderState &render_dev = Device::GetInstance()->render_state();
    render_dev.Set3D();

    projection_mat = render_dev.projection_mat();
    view_mat = render_dev.view_mat();
    model_mat = render_dev.model_mat();
  }

  virtual ~DebugDrawCmd() {}

  DebugDrawType type;

  //shared
  sora::vec4ub color;
  float duration;
  bool depth_enable;
  
  glm::mat4 projection_mat;
  glm::mat4 view_mat;
  glm::mat4 model_mat;

  glm::mat4 GetMVPMatrix() const { return projection_mat * view_mat * model_mat; }
};

struct DebugDrawCmd_Line : public DebugDrawCmd {
  DebugDrawCmd_Line() : DebugDrawCmd(kDebugDrawLine), line_width(1.0f) {}
  float line_width;
  glm::vec3 p1, p2;   //line
};

struct DebugDrawCmd_Cross : public DebugDrawCmd {
  DebugDrawCmd_Cross() : DebugDrawCmd(kDebugDrawCross), size(1.0f) {}
  float size;
  glm::vec3 pos;  //cross, sphere, string
};
struct DebugDrawCmd_Sphere : public DebugDrawCmd {
  DebugDrawCmd_Sphere() : DebugDrawCmd(kDebugDrawSphere), radius(0) {}
  float radius;
  glm::vec3 pos;  //cross, sphere, string
};
struct DebugDrawCmd_String : public DebugDrawCmd {
  DebugDrawCmd_String() : DebugDrawCmd(kDebugDrawString), scale(1.0f) {}
  std::string msg;
  float scale;
  glm::vec3 pos;  //cross, sphere, string
};
struct DebugDrawCmd_Axis : public DebugDrawCmd {
  DebugDrawCmd_Axis() : DebugDrawCmd(kDebugDrawAxis), size(1.0f) {}
  float size;
  glm::mat4 xf;   //axis
};


void DebugDrawManager::AddLine(const glm::vec3 &p1, const glm::vec3 &p2,
                               const sora::vec4ub &color,
                               float line_width,
                               float duration,
                               bool depth_enable) {
  DebugDrawCmd_Line *cmd = sora::global_malloc<DebugDrawCmd_Line>();
  new(cmd) DebugDrawCmd_Line;

  cmd->p1 = p1;
  cmd->p2 = p2;
  cmd->color = color;
  cmd->line_width = line_width;
  cmd->duration = duration;
  cmd->depth_enable = depth_enable;
  cmd_list_.push_back(cmd);
}

void DebugDrawManager::AddCross(const glm::vec3 &pos, 
                                const sora::vec4ub &color,
                                float size,
                                float duration,
                                bool depth_enable) {
  DebugDrawCmd_Cross *cmd = sora::global_malloc<DebugDrawCmd_Cross>();
  new(cmd) DebugDrawCmd_Cross;

  cmd->pos = pos;
  cmd->color = color;
  cmd->size = size;
  cmd->duration = duration;
  cmd->depth_enable = depth_enable;
  cmd_list_.push_back(cmd);
}

void DebugDrawManager::AddSphere(const glm::vec3 &pos, float radius,
                                 const sora::vec4ub &color,
                                 float duration,
                                 bool depth_enable) {
  DebugDrawCmd_Sphere *cmd = sora::global_malloc<DebugDrawCmd_Sphere>();
  new(cmd) DebugDrawCmd_Sphere;
  
  cmd->pos = pos;
  cmd->radius = radius;
  cmd->color = color;
  cmd->duration = duration;
  cmd->depth_enable = depth_enable;
  cmd_list_.push_back(cmd);
}

void DebugDrawManager::AddAxis(const glm::mat4 &xf,
                               float size, 
                               float duration,
                               bool depth_enable) {
  DebugDrawCmd_Axis *cmd = sora::global_malloc<DebugDrawCmd_Axis>();
  CallConstructor(cmd);
  
  cmd->xf = xf;
  cmd->size = size;
  cmd->duration = duration;
  cmd->depth_enable = depth_enable;
  cmd_list_.push_back(cmd);
}

void DebugDrawManager::AddString(const glm::vec3 &pos, const std::string &msg,
                                 const sora::vec4ub &color,
                                 float scale,
                                 float duration,
                                 bool depth_enable) {
  DebugDrawCmd_String *cmd = sora::global_malloc<DebugDrawCmd_String>();
  CallConstructor(cmd);

  cmd->pos = pos;
  cmd->msg = msg;
  cmd->color = color;
  cmd->scale = scale;
  cmd->duration = duration;
  cmd->depth_enable = depth_enable;
  cmd_list_.push_back(cmd);
}

DebugDrawManager::DebugDrawManager() {

}
DebugDrawManager::~DebugDrawManager() {
  Clear();
}

void DebugDrawManager::Clear() {
  auto it = cmd_list_.begin();
  auto endit = cmd_list_.end();
  for( ; it != endit ; ++it) {
    DebugDrawCmd *cmd = *it;
    cmd->~DebugDrawCmd();
    sora::global_free(cmd);
  }
  cmd_list_.clear();
}

void DebugDrawManager::Update(float dt) {
  struct RemoveFindFunctor {
    bool operator()(DebugDrawCmd *cmd) {
      return (cmd->duration < 0);
    }
  };
  RemoveFindFunctor functor;

  auto it = cmd_list_.begin();
  while(it != cmd_list_.end()) {
    DebugDrawCmd *cmd = *it;
    cmd->duration -= dt;
    
    if(functor(cmd) == true) {
      it = cmd_list_.erase(it);
      cmd->~DebugDrawCmd();
      sora::global_free(cmd);
      
    } else {
      it++;
    }
  }
}

//////////////////////////////////////

void DebugDrawPolicy::DrawCmdList(const DebugDrawManager &mgr) {

  auto it = mgr.cmd_list_.begin();
  auto endit = mgr.cmd_list_.end();
  for( ; it != endit ; ++it) {
    Draw(*it);
  }
}

void DebugDrawPolicy::Draw(const DebugDrawManager &mgr) {
  mgr_ = const_cast<DebugDrawManager*>(&mgr);

  RenderState *dev = &Device::GetInstance()->render_state();
  dev->Set3D();

  DrawCmdList(mgr);

  mgr_ = NULL;
}


void DebugDrawPolicy::Draw(DebugDrawCmd *cmd) {
  switch(cmd->type) {
  case DebugDrawCmd::kDebugDrawLine:
    DrawElem(static_cast<DebugDrawCmd_Line*>(cmd));
    break;
  case DebugDrawCmd::kDebugDrawCross:
    DrawElem(static_cast<DebugDrawCmd_Cross*>(cmd));
    break;
  case DebugDrawCmd::kDebugDrawSphere:
    DrawElem(static_cast<DebugDrawCmd_Sphere*>(cmd));
    break;
  case DebugDrawCmd::kDebugDrawString:
    DrawElem(static_cast<DebugDrawCmd_String*>(cmd));
    break;
  case DebugDrawCmd::kDebugDrawAxis:
    DrawElem(static_cast<DebugDrawCmd_Axis*>(cmd));
    break;
  default:
    break;
  }
}


void DebugDrawPolicy::ApplyDepthTest(DebugDrawCmd *cmd) {
  if(cmd->depth_enable == false) {
    glDisable(GL_DEPTH_TEST);
  }
}
void DebugDrawPolicy::UnapplyDepthTest(DebugDrawCmd *cmd) {
  if(cmd->depth_enable == false) {
    glEnable(GL_DEPTH_TEST);
  }
}

///////////////////////////////////////////////

void DebugDrawPolicy::DrawElem(DebugDrawCmd_Line *cmd) {
  ShaderManager *shader_mgr = Device::GetInstance()->shader_mgr();
  Shader *shader = shader_mgr->Get(ShaderManager::kConstColor);
  RenderState *render_state = &Device::GetInstance()->render_state();
  render_state->UseShader(*shader);

  vec4 color = Draw2DPolicy::ConvertColor(cmd->color);
  bool const_color_result = shader->SetUniformVector(kConstColorHandleName, color);
  SR_ASSERT(const_color_result == true);

  vector<vec3> vertex_list;
  vertex_list.push_back(cmd->p1);
  vertex_list.push_back(cmd->p2);

  mat4 mvp = cmd->GetMVPMatrix();
  ShaderVariable mvp_var = shader->uniform_var(kMVPHandleName);
  SR_ASSERT(mvp_var.location != kInvalidShaderVarLocation);
  SetUniformMatrix(mvp_var, mvp);

  shader->SetVertexList(vertex_list);
  glLineWidth(cmd->line_width);
  ApplyDepthTest(cmd);
  shader->DrawArrays(kDrawLines, vertex_list.size());  
  UnapplyDepthTest(cmd);
  glLineWidth(1.0f);
}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_Cross *cmd) {
  ShaderManager *shader_mgr = Device::GetInstance()->shader_mgr();
  Shader &shader = *(shader_mgr->Get(ShaderManager::kConstColor));
  RenderState *dev = &Device::GetInstance()->render_state();
  dev->UseShader(shader);

  vec4 color = Draw2DPolicy::ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  mat4 mvp = cmd->GetMVPMatrix();
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp);

  vector<glm::vec3> vert_list;
  vert_list.push_back(cmd->pos);

  shader.SetVertexList(vert_list);

  ApplyDepthTest(cmd);
  glPointSize(cmd->size);
  shader.DrawArrays(kDrawPoints, vert_list.size());
  glPointSize(1.0f);
  UnapplyDepthTest(cmd);
}

void DebugDrawPolicy::DrawElem(DebugDrawCmd_Sphere *cmd) {
  ShaderManager *shader_mgr = Device::GetInstance()->shader_mgr();
  Shader &shader = *(shader_mgr->Get(ShaderManager::kConstColor));
  RenderState *dev = &Device::GetInstance()->render_state();
  dev->UseShader(shader);

  vec4 color = Draw2DPolicy::ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  static bool init_mesh = false;
  static GeometricObject<glm::vec3> mesh;
  if(init_mesh == false) {
    init_mesh = true;
    mesh.WireSphere(1.0f, 16, 16);
  }

  mat4 mvp = cmd->GetMVPMatrix();
  mvp = glm::translate(mvp, cmd->pos);
  mvp = glm::scale(mvp, vec3(cmd->radius, cmd->radius, cmd->radius));
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp);

  ApplyDepthTest(cmd);
  auto it = mesh.Begin();
  auto endit = mesh.End();
  for( ; it != endit ; ++it) {
    //const DrawCmdData<Vertex> &cmd = *it;
    const DrawCmdData<vec3> &cmd = *it;
    shader.SetVertexList(cmd.vertex_list);
    if(cmd.index_list.empty()) {
      Shader::DrawArrays(cmd.draw_mode, cmd.vertex_list.size());
    } else {
      Shader::DrawElements(cmd.draw_mode, cmd.index_list);
    }
  }
  UnapplyDepthTest(cmd);
}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_String *cmd) {
  //billboard 같은 느낌으로 글자 쓰기
  //기울어지는거 없이 항상 글자가 뜨도록 적절히 만들기
  mat4 mvp = cmd->GetMVPMatrix();
  vec4 cliping_pos = mvp * vec4(cmd->pos, 1);   // -1~1에 적절히 위치한 좌표(로 아마도 보정되엇을거다)
  cliping_pos.x /= cliping_pos.w;
  cliping_pos.y /= cliping_pos.w;

  RenderState *dev = &Device::GetInstance()->render_state();
  float win_w = (float)dev->win_width();
  float win_h = (float)dev->win_height();
  // -1~+1로 보정된 좌표를 윈도우좌표로 변환
  vec3 win_coord((cliping_pos.x + 1) * win_w/2.0f, (cliping_pos.y + 1.0f) * win_h/2.0f, 0);
  
  ShaderManager *shader_mgr = Device::GetInstance()->shader_mgr();
  Shader &shader = *(shader_mgr->Get(ShaderManager::kText));
  dev->UseShader(shader);

  vec4 color = Draw2DPolicy::ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  sora::SysFont *font = Device::GetInstance()->sys_font();
  dev->UseTexture(font->font_texture(), 0);

  mat4 mvp_mat = glm::ortho(0.0f, win_w, 0.0f, win_h);
  mvp_mat = glm::translate(mvp_mat, win_coord);
  mvp_mat = glm::scale(mvp_mat, vec3(cmd->scale));
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp_mat);
  
  //2d처럼 렌더링하기
  glEnable(GL_BLEND);

  ApplyDepthTest(cmd);
  sora::Label label(font, cmd->msg);
  shader.SetVertexList(label.vertex_list());
  shader.DrawElements(kDrawTriangles, label.index_list());
  UnapplyDepthTest(cmd);

  //3d용으로 복구
  glDisable(GL_BLEND);

  //점하나 적절히 찍기
  DebugDrawCmd_Cross cross_cmd;
  cross_cmd.color = cmd->color;
  cross_cmd.depth_enable = cmd->depth_enable;
  cross_cmd.duration = cmd->duration;
  cross_cmd.model_mat = cmd->model_mat;
  cross_cmd.view_mat = cmd->view_mat;
  cross_cmd.projection_mat = cmd->projection_mat;
  cross_cmd.pos = cmd->pos;
  cross_cmd.size = 5;
  DrawElem(&cross_cmd);
}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_Axis *cmd) {
  ShaderManager *shader_mgr = Device::GetInstance()->shader_mgr();
  Shader *shader = shader_mgr->Get(ShaderManager::kVertexColor);
  RenderState *render_state = &Device::GetInstance()->render_state();
  render_state->UseShader(*shader);

  vector<ColorVertex3D> vertex_list;
  vertex_list.reserve(6);
  
  vec4ub red(255, 0, 0, 255);
  vec4ub green(0, 255, 0, 255);
  vec4ub blue(0, 0, 255, 255);

  // x = red
  vertex_list.push_back(CreateColorVertex3D(0, 0, 0, red));
  vertex_list.push_back(CreateColorVertex3D(cmd->size, 0, 0, red));
  
  // y = green
  vertex_list.push_back(CreateColorVertex3D(0, 0, 0, green));
  vertex_list.push_back(CreateColorVertex3D(0, cmd->size, 0, green));

  // z = blue
  vertex_list.push_back(CreateColorVertex3D(0, 0, 0, blue));
  vertex_list.push_back(CreateColorVertex3D(0, 0, cmd->size, blue));

  mat4 mvp = cmd->GetMVPMatrix() * cmd->xf;
  ShaderVariable mvp_var = shader->uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp);

  ApplyDepthTest(cmd);
  shader->SetVertexList(vertex_list);
  shader->DrawArrays(kDrawLines, vertex_list.size());  
  UnapplyDepthTest(cmd);
}

} //namespace sora