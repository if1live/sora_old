/*  Copyright (C) 2011 by if1live */
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
#include "precompile.h"
#include "runa/ui_draw_helper.h"
#include "matsu/vector.h"
#include "matsu/matrix.h"
#include "runa/basic_color_shader.h"
#include "matsu/rectangle.h"
#include "runa/window.h"
#include "runa/gl_tool.h"

using matsu::vec2;
using matsu::vec4;
using std::vector;

namespace runa {;
void UIDrawHelper::DrawLine(const matsu::vec2 &p1,
  const matsu::vec2 &p2, const matsu::vec4 &color) {
  vector<vec2> ui_point_list;
  ui_point_list.reserve(2);
  ui_point_list.push_back(p1);
  ui_point_list.push_back(p2);
  DrawLine(ui_point_list, color);
}
void UIDrawHelper::DrawLine(const std::vector<matsu::vec2> &ui_point_list,
    const matsu::vec4 &color) {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  shader.Use();

  shader.SetColor4fv(color.Pointer());

  // apply projection
  matsu::mat4 projection;
  int win_w = runa::Window::GetInstance().width();
  int win_h = runa::Window::GetInstance().height();
  projection = matsu::Matrix::Ortho<float>(0, win_w, 0, win_h, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  shader.SetMatrix(projection.Pointer());

  vector<vec2> gl_point_list;
  gl_point_list.reserve(ui_point_list.size());

  BOOST_FOREACH(const vec2 &ui_point, ui_point_list) {
    float x = ui_point.x();
    float y = win_h - ui_point.y();
    gl_point_list.push_back(vec2(x, y));
  }

  GLint position_location = shader.position_location();
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, gl_point_list[0].Pointer());
  glDrawArrays(GL_LINE_STRIP, 0, gl_point_list.size());
}
void UIDrawHelper::DrawLineRect(const matsu::Rectf &rect, const matsu::vec4 &color) {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  shader.Use();

  shader.SetColor4fv(color.Pointer());

  // apply projection
  matsu::mat4 projection;
  int win_w = runa::Window::GetInstance().width();
  int win_h = runa::Window::GetInstance().height();
  projection = matsu::Matrix::Ortho<float>(0, win_w, 0, win_h, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(rect.x, win_h - rect.y, -1);
  shader.SetMatrix(projection.Pointer());

  float left = 0;
  float right = left + rect.width;
  float top = 0;
  float bottom = top - rect.height;
  float vertex[] = {
    left, bottom,
    right, bottom,
    right, top,
    left, top
  };
  GLint position_location = shader.position_location();
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  unsigned char index[] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0
  };
  glDrawElements(GL_LINES, sizeof(index) / sizeof(index[0]), GL_UNSIGNED_BYTE, index);
  SR_ASSERT(runa::GLTool::CheckError("draw line rect"));
}

void UIDrawHelper::DrawRect(const matsu::Rectf &rect, const matsu::vec4 &color) {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  shader.Use();

  shader.SetColor4fv(color.Pointer());

  // apply projection
  matsu::mat4 projection;
  int win_w = runa::Window::GetInstance().width();
  int win_h = runa::Window::GetInstance().height();
  projection = matsu::Matrix::Ortho<float>(0, win_w, 0, win_h, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(rect.x, win_h - rect.y, -1);
  shader.SetMatrix(projection.Pointer());

  float left = 0;
  float right = left + rect.width;
  float top = 0;
  float bottom = top - rect.height;
  float vertex[] = {
    left, bottom,
    right, bottom,
    right, top,
    left, top
  };
  GLint position_location = shader.position_location();
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3
  };
  glDrawElements(GL_TRIANGLES, sizeof(index) / sizeof(index[0]), GL_UNSIGNED_BYTE, index);
  SR_ASSERT(runa::GLTool::CheckError("draw rect"));
}
}
