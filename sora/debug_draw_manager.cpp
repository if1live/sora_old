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
#include "filesystem.h"
#include "render_device.h"
#include "geometric_object.h"

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

  DebugDrawCmd() :
    type(kDebugDrawNone),
    duration(0),
    depth_enable(true) {}

  DebugDrawCmd(DebugDrawType type) :
    type(type),
    duration(0),
    depth_enable(true) {}

  virtual ~DebugDrawCmd() {}

  DebugDrawType type;

  //shared
  sora::vec4ub color;
  float duration;
  bool depth_enable;
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
                               const sora::vec4ub &color,
                               float size, 
                               float duration,
                               bool depth_enable) {
  DebugDrawCmd_Axis *cmd = sora::global_malloc<DebugDrawCmd_Axis>();
  CallConstructor(cmd);
  
  cmd->xf = xf;
  cmd->color = color;
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
  auto endit = cmd_list_.end();
  for( ; it != endit ; ++it) {
    DebugDrawCmd *cmd = *it;
    cmd->duration -= dt;
    
    if(functor(cmd) == true) {
      cmd->~DebugDrawCmd();
      sora::global_free(cmd);
    }
  }
  
  cmd_list_.remove_if(functor);
}


DebugDrawManager &DebugDrawManager::Get2D() {
  static DebugDrawManager mgr;
  return mgr;
}
DebugDrawManager &DebugDrawManager::Get3D() {
  static DebugDrawManager mgr;
  return mgr;
}
Shader &DebugDrawManager::GetColorShader() {
  static Shader shader;
  static bool run = false;
  if(run == false) {
    run = true;
    string color_vs_path = Filesystem::GetAppPath("shader/const_color.vs");
    string color_fs_path = Filesystem::GetAppPath("shader/const_color.fs");
    shader.LoadFromFile(color_vs_path, color_fs_path);
  }
  return shader;
}
Shader &DebugDrawManager::GetTextShader() {
  static Shader shader;
  static bool run = false;
  if(run == false) {
    run = true;
    string simple_vs_path = Filesystem::GetAppPath("shader/simple.vs");
    string simple_fs_path = Filesystem::GetAppPath("shader/text.fs");
    shader.LoadFromFile(simple_vs_path, simple_fs_path);
  }
  return shader;
}

//////////////////////////////////////

void DebugDrawPolicy::DrawCmdList(const DebugDrawManager &mgr) {

  auto it = mgr.cmd_list_.begin();
  auto endit = mgr.cmd_list_.end();
  for( ; it != endit ; ++it) {
    Draw(*it);
  }
}

void DebugDrawPolicy::Draw(const DebugDrawManager &mgr, RenderDevice *dev) {
  mgr_ = const_cast<DebugDrawManager*>(&mgr);
  dev_ = dev;

  BeforeDraw();
  DrawCmdList(mgr);

  mgr_ = NULL;
  dev_ = NULL;
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
glm::vec4 DebugDrawPolicy::ConvertColor(const sora::vec4ub &orig) {
  vec4 color;
  for(int i = 0 ; i < 4 ; i++) {
    color[i] = (float)orig[i] / 255.0f;
  }
  return color;
}

///////////////////////////////
void DebugDrawPolicy_2D::BeforeDraw() {
  dev()->Set2D();
}

void DebugDrawPolicy_2D::DrawElem(DebugDrawCmd_Line *cmd) {
  Shader &shader = DebugDrawManager::GetColorShader();
  dev()->UseShader(shader);

  vec4 color = ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  float win_width = (float)dev()->win_width();
  float win_height = (float)dev()->win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, projection);

  vector<glm::vec3> vert_list;
  vert_list.push_back(cmd->p1);
  vert_list.push_back(cmd->p2);

  shader.SetVertexList(vert_list);

  ApplyDepthTest(cmd);
  glLineWidth(cmd->line_width);
  shader.DrawArrays(kDrawLines, 2);
  glLineWidth(1.0f);
  UnapplyDepthTest(cmd);
}
void DebugDrawPolicy_2D::DrawElem(DebugDrawCmd_Cross *cmd) {
  Shader &shader = DebugDrawManager::GetColorShader();
  dev()->UseShader(shader);

  vec4 color = ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  float win_width = (float)dev()->win_width();
  float win_height = (float)dev()->win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, projection);

  vector<glm::vec3> vert_list;
  vert_list.push_back(cmd->pos);

  shader.SetVertexList(vert_list);

  ApplyDepthTest(cmd);
  glPointSize(cmd->size);
  shader.DrawArrays(kDrawPoints, 2);
  glPointSize(1.0f);
  UnapplyDepthTest(cmd);

}
void DebugDrawPolicy_2D::DrawElem(DebugDrawCmd_Sphere *cmd) {
  Shader &shader = DebugDrawManager::GetColorShader();
  dev()->UseShader(shader);

  vec4 color = ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  float win_width = (float)dev()->win_width();
  float win_height = (float)dev()->win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
  glm::mat4 mvp = glm::translate(projection, cmd->pos);
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, mvp);

  GeometricObject<glm::vec3> mesh;
  mesh.WireSphere(cmd->radius, 16, 16);

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
void DebugDrawPolicy_2D::DrawElem(DebugDrawCmd_String *cmd) {
  Shader &shader = DebugDrawManager::GetTextShader();
  dev()->UseShader(shader);

  vec4 color = ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  sora::SysFont &font = dev()->sys_font();
  dev()->UseTexture(font.font_texture());

  //해상도에 맞춰서 적절히 설정
  float win_width = (float)dev()->win_width();
  float win_height = (float)dev()->win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);

  mat4 world_mat(1.0f);
  world_mat = glm::translate(world_mat, cmd->pos);
  world_mat = glm::scale(world_mat, vec3(cmd->scale));
  mat4 mvp = projection * world_mat;
  SetUniformMatrix(mvp_var, mvp);
  sora::Label label(&font, "PQRS_1234_asdf");
  shader.SetVertexList(label.vertex_list());

  ApplyDepthTest(cmd);
  shader.DrawElements(kDrawTriangles, label.index_list());
  UnapplyDepthTest(cmd);
}
void DebugDrawPolicy_2D::DrawElem(DebugDrawCmd_Axis *cmd) {
  Shader &shader = DebugDrawManager::GetColorShader();
  dev()->UseShader(shader);

}


///////////////////////////////////////////////


void DebugDrawPolicy_3D::BeforeDraw() {

}
void DebugDrawPolicy_3D::DrawElem(DebugDrawCmd_Line *cmd) {

}
void DebugDrawPolicy_3D::DrawElem(DebugDrawCmd_Cross *cmd) {

}
void DebugDrawPolicy_3D::DrawElem(DebugDrawCmd_Sphere *cmd) {

}
void DebugDrawPolicy_3D::DrawElem(DebugDrawCmd_String *cmd) {

}
void DebugDrawPolicy_3D::DrawElem(DebugDrawCmd_Axis *cmd) {

}

} //namespace sora