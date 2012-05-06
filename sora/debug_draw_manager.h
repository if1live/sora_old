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
#pragma once

#include "vector.h"
#include <list>
#include "globals.h"

namespace sora {;

struct DebugDrawCmd;

struct DebugDrawCmd_Line;
struct DebugDrawCmd_Cross;
struct DebugDrawCmd_Sphere;
struct DebugDrawCmd_String;
struct DebugDrawCmd_Axis;

class DebugDrawPolicy_2D;
class DebugDrawPolicy_3D;

class DebugDrawManager;

struct DebugDrawPolicy {
public:
  DebugDrawPolicy() : mgr_(NULL), dev_(NULL) {}
  ~DebugDrawPolicy() {}

public:
  void Draw(const DebugDrawManager &mgr, RenderDevice *dev);
  void DrawCmdList(const DebugDrawManager &mgr);
  void Draw(DebugDrawCmd *cmd);

protected:
  virtual void BeforeDraw() = 0;
  virtual void DrawElem(DebugDrawCmd_Line *cmd) = 0;
  virtual void DrawElem(DebugDrawCmd_Cross *cmd) = 0;
  virtual void DrawElem(DebugDrawCmd_Sphere *cmd) = 0;
  virtual void DrawElem(DebugDrawCmd_String *cmd) = 0;
  virtual void DrawElem(DebugDrawCmd_Axis *cmd) = 0;

  DebugDrawManager *mgr() { return mgr_; }
  RenderDevice *dev() { return dev_; }

  void ApplyDepthTest(DebugDrawCmd *cmd);
  void UnapplyDepthTest(DebugDrawCmd *cmd);
  glm::vec4 ConvertColor(const sora::vec4ub &orig);

private:
  DebugDrawManager *mgr_;
  RenderDevice *dev_;
};

class DebugDrawPolicy_2D : public DebugDrawPolicy {
private:
  void BeforeDraw();
  void DrawElem(DebugDrawCmd_Line *cmd);
  void DrawElem(DebugDrawCmd_Cross *cmd);
  void DrawElem(DebugDrawCmd_Sphere *cmd);
  void DrawElem(DebugDrawCmd_String *cmd);
  void DrawElem(DebugDrawCmd_Axis *cmd);
};

class DebugDrawPolicy_3D : public DebugDrawPolicy {
private:
  void BeforeDraw();
  void DrawElem(DebugDrawCmd_Line *cmd);
  void DrawElem(DebugDrawCmd_Cross *cmd);
  void DrawElem(DebugDrawCmd_Sphere *cmd);
  void DrawElem(DebugDrawCmd_String *cmd);
  void DrawElem(DebugDrawCmd_Axis *cmd);
};

class DebugDrawManager {;
public:
  friend class DebugDrawPolicy;

  static DebugDrawManager &Get2D();
  static DebugDrawManager &Get3D();
  static Shader &GetColorShader();
  static Shader &GetTextShader();

public:
  DebugDrawManager();
  ~DebugDrawManager();
  void Update(float dt);
  int CmdCount() const { return cmd_list_.size(); }
  void Clear();

  void AddLine(const glm::vec3 &p1, const glm::vec3 &p2,
    const sora::vec4ub &color,
    float line_width = 1.0f,
    float duration = 0.0f,
    bool depth_enable = true);

  void AddCross(const glm::vec3 &pos, 
    const sora::vec4ub &color,
    float size,
    float duration = 0.0f,
    bool depth_enable = true);

  void AddSphere(const glm::vec3 &pos, float radius,
    const sora::vec4ub &color,
    float duration = 0.0f,
    bool depth_enable = true);

  void AddAxis(const glm::mat4 &xf,
    float size, 
    float duration = 0.0f,
    bool depth_enable = true);

  void AddString(const glm::vec3 &pos, const std::string &msg,
    const sora::vec4ub &color,
    float scale = 1.0f,
    float duration = 0.0f,
    bool depth_enable = true);
  
private:
  std::list<DebugDrawCmd*> cmd_list_;
};

} //namespace sora