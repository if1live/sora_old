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

using namespace std;

namespace sora {;

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
  DebugDrawCmd_String() : DebugDrawCmd(kDebugDrawString) {}
  std::string msg;
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
                                 float duration,
                                 bool depth_enable) {
  DebugDrawCmd_String *cmd = sora::global_malloc<DebugDrawCmd_String>();
  CallConstructor(cmd);

  cmd->pos = pos;
  cmd->msg = msg;
  cmd->color = color;
  cmd->duration = duration;
  cmd->depth_enable = depth_enable;
  cmd_list_.push_back(cmd);
}

DebugDrawManager::DebugDrawManager() {

}
DebugDrawManager::~DebugDrawManager() {
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

} //namespace sora