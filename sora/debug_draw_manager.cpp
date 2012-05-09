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
#include "debug_draw_manager.h"
#include "template_lib.h"
#include "shader.h"
#include "render_device.h"
#include "geometric_object.h"
#include "matrix_stack.h"
#include "device.h"
#include "render_device.h"

#include "draw_2d_manager.h"

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
    RenderDevice &render_dev = Device::GetInstance()->render_device();
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

  BeforeDraw();
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

///////////////////////////////


///////////////////////////////////////////////


void DebugDrawPolicy::BeforeDraw() {

}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_Line *cmd) {

}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_Cross *cmd) {

}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_Sphere *cmd) {

}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_String *cmd) {

}
void DebugDrawPolicy::DrawElem(DebugDrawCmd_Axis *cmd) {

}

} //namespace sora