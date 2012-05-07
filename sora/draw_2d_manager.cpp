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
#include "draw_2d_manager.h"
#include "debug_draw_manager.h"
#include "shader.h"
#include "render_device.h"
#include "filesystem.h"

using namespace std;
using namespace glm;

namespace sora {;

typedef enum {
  kDraw2DNone,
  kDraw2DLine,
  kDraw2DCross,
  kDraw2DString,
} Draw2DType;

struct DrawCmd2D {
  DrawCmd2D() : type(kDraw2DNone), duration(0) {}
  DrawCmd2D(Draw2DType type)  : type(type), duration(0) {}
  Draw2DType type;
  vec4ub color;
  float duration;
};
struct DrawCmd2D_Line : public DrawCmd2D {
  DrawCmd2D_Line() : DrawCmd2D(kDraw2DLine), line_width(1.0f) {}
  vec2 p1;
  vec2 p2;
  float line_width;
};
struct DrawCmd2D_Cross : public DrawCmd2D {
  DrawCmd2D_Cross() : DrawCmd2D(kDraw2DCross), size(5.0f) {}
  vec2 pos;
  float size;
};
struct DrawCmd2D_String : public DrawCmd2D {
  DrawCmd2D_String() : DrawCmd2D(kDraw2DString), scale(1.0f) {}
  vec2 pos;
  std::string msg;
  float scale;
};

Draw2DManager::Draw2DManager() {

}
Draw2DManager::~Draw2DManager() {
  Clear();
}
void Draw2DManager::Clear() {
  auto it = cmd_list_.begin();
  auto endit = cmd_list_.end();
  for( ; it != endit ; ++it) {
    DrawCmd2D *cmd = *it;
    cmd->~DrawCmd2D();
    sora::global_free(cmd);
  }
  cmd_list_.clear();
}
void Draw2DManager::Update(float dt) {
  struct RemoveFindFunctor {
    bool operator()(DrawCmd2D *cmd) {
      return (cmd->duration < 0);
    }
  };
  RemoveFindFunctor functor;

  auto it = cmd_list_.begin();
  auto endit = cmd_list_.end();
  for( ; it != endit ; ++it) {
    DrawCmd2D *cmd = *it;
    cmd->duration -= dt;

    if(functor(cmd) == true) {
      cmd->~DrawCmd2D();
      sora::global_free(cmd);
    }
  }

  cmd_list_.remove_if(functor);
}

void Draw2DManager::AddLine(const glm::vec2 &p1, const glm::vec2 &p2,
             const sora::vec4ub &color,
             float line_width,
             float duration) {
  DrawCmd2D_Line *cmd = sora::global_malloc<DrawCmd2D_Line>();
  new(cmd) DrawCmd2D_Line;
  cmd->p1 = p1;
  cmd->p2 = p2;
  cmd->color = color;
  cmd->line_width = line_width;
  cmd->duration = duration;
  cmd_list_.push_back(cmd);
}

void Draw2DManager::AddCross(const glm::vec2 &pos, 
              const sora::vec4ub &color,
              float size,
              float duration) {
  DrawCmd2D_Cross *cmd = sora::global_malloc<DrawCmd2D_Cross>();
  new(cmd) DrawCmd2D_Cross;
  cmd->pos = pos;
  cmd->color = color;
  cmd->size = size;
  cmd->duration = duration;
  cmd_list_.push_back(cmd);
}

void Draw2DManager::AddString(const glm::vec2 &pos, const std::string &msg,
               const sora::vec4ub &color,
               float scale,
               float duration) {
  DrawCmd2D_String *cmd = sora::global_malloc<DrawCmd2D_String>();
  new(cmd) DrawCmd2D_String;
  cmd->pos = pos;
  cmd->msg = msg;
  cmd->color = color;
  cmd->scale = scale;
  cmd->duration = duration;
  cmd_list_.push_back(cmd);
}

///////////////////////////////////////

void Draw2DPolicy::BeforeDraw() {
  dev()->Set2D();
}

void Draw2DPolicy::DrawElem(DrawCmd2D_Line *cmd) {
  Shader &shader = GetColorShader();
  dev()->UseShader(shader);

  vec4 color = ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  float win_width = (float)dev()->win_width();
  float win_height = (float)dev()->win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, projection);

  vector<glm::vec2> vert_list;
  vert_list.push_back(cmd->p1);
  vert_list.push_back(cmd->p2);

  shader.SetVertexList(vert_list);

  glLineWidth(cmd->line_width);
  shader.DrawArrays(kDrawLines, 2);
  glLineWidth(1.0f);
}
void Draw2DPolicy::DrawElem(DrawCmd2D_Cross *cmd) {
  Shader &shader = GetColorShader();
  dev()->UseShader(shader);

  vec4 color = ConvertColor(cmd->color);
  shader.SetUniformVector(kConstColorHandleName, color);

  float win_width = (float)dev()->win_width();
  float win_height = (float)dev()->win_height();
  glm::mat4 projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  SetUniformMatrix(mvp_var, projection);

  vector<glm::vec2> vert_list;
  vert_list.push_back(cmd->pos);

  shader.SetVertexList(vert_list);

  glPointSize(cmd->size);
  shader.DrawArrays(kDrawPoints, 2);
  glPointSize(1.0f);
}
/*
void Draw2DPolicy::DrawElem(DebugDrawCmd_Sphere *cmd) {
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
*/
void Draw2DPolicy::DrawElem(DrawCmd2D_String *cmd) {
  Shader &shader = GetTextShader();
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
  world_mat = glm::translate(world_mat, vec3(cmd->pos.x, cmd->pos.y, 0));
  world_mat = glm::scale(world_mat, vec3(cmd->scale));
  mat4 mvp = projection * world_mat;
  SetUniformMatrix(mvp_var, mvp);
  sora::Label label(&font, cmd->msg);
  shader.SetVertexList(label.vertex_list());

  shader.DrawElements(kDrawTriangles, label.index_list());
}

glm::vec4 Draw2DPolicy::ConvertColor(const sora::vec4ub &orig) {
  vec4 color;
  for(int i = 0 ; i < 4 ; i++) {
    color[i] = (float)orig[i] / 255.0f;
  }
  return color;
}


void Draw2DPolicy::DrawCmdList(const Draw2DManager &mgr) {

  auto it = mgr.cmd_list_.begin();
  auto endit = mgr.cmd_list_.end();
  for( ; it != endit ; ++it) {
    Draw(*it);
  }
}

void Draw2DPolicy::Draw(const Draw2DManager &mgr, RenderDevice *dev) {
  mgr_ = const_cast<Draw2DManager*>(&mgr);
  dev_ = dev;

  BeforeDraw();
  DrawCmdList(mgr);

  mgr_ = NULL;
  dev_ = NULL;
}


void Draw2DPolicy::Draw(DrawCmd2D *cmd) {
  switch(cmd->type) {
  case kDraw2DLine:
    DrawElem(static_cast<DrawCmd2D_Line*>(cmd));
    break;
  case kDraw2DCross:
    DrawElem(static_cast<DrawCmd2D_Cross*>(cmd));
    break;
  case kDraw2DString:
    DrawElem(static_cast<DrawCmd2D_String*>(cmd));
    break;
  default:
    break;
  }
}


Shader &Draw2DPolicy::GetColorShader() {
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
Shader &Draw2DPolicy::GetTextShader() {
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

} //namespace sora