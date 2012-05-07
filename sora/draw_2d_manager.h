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
#include "globals.h"

namespace sora {;

struct DrawCmd2D;
struct DrawCmd2D_Line;
struct DrawCmd2D_Cross;
struct DrawCmd2D_String;

class Draw2DPolicy;
class Draw2DManager;

class Draw2DManager {
public:
  friend class Draw2DPolicy;
  Draw2DManager();
  ~Draw2DManager();

  void Clear();
  void Update(float dt);
public:
  void AddLine(const glm::vec2 &p1, const glm::vec2 &p2,
    const sora::vec4ub &color,
    float line_width = 1.0f,
    float duration = 0.0f);

  void AddCross(const glm::vec2 &pos, 
    const sora::vec4ub &color,
    float size,
    float duration = 0.0f);

  void AddString(const glm::vec2 &pos, const std::string &msg,
    const sora::vec4ub &color,
    float scale = 1.0f,
    float duration = 0.0f);
private:
  std::list<DrawCmd2D*> cmd_list_;
};

class Draw2DPolicy {
public:
  Draw2DPolicy() : mgr_(NULL), dev_(NULL) {}
  ~Draw2DPolicy() {}

  static Shader &GetColorShader();
  static Shader &GetTextShader();

public:
  void Draw(const Draw2DManager &mgr, RenderDevice *dev);
  void DrawCmdList(const Draw2DManager &mgr);
  void Draw(DrawCmd2D *cmd);

private:
  void BeforeDraw();
  void DrawElem(DrawCmd2D_Line *cmd);
  void DrawElem(DrawCmd2D_Cross *cmd);
  void DrawElem(DrawCmd2D_String *cmd);

  Draw2DManager *mgr() { return mgr_; }
  RenderDevice *dev() { return dev_; }
  glm::vec4 ConvertColor(const sora::vec4ub &orig);
private:
  Draw2DManager *mgr_;
  RenderDevice *dev_;
};

} // namespace sora