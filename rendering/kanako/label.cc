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
#include "kanako/label.h"
#include "kanako/font.h"
#include "runa/basic_texture_shader.h"
#include "runa/basic_color_shader.h"
#include "matsu/matrix.h"
#include "runa/gl_tool.h"
#include "runa/window.h"
#include "runa/ui_draw_helper.h"
#include "matsu/rectangle.h"

using runa::BasicTextureShader;
using runa::BasicColorShader;

namespace kanako {;

Label::Label()
: scale(1) {
}
Label::Label(const std::string &msg)
  : text(msg), scale(1) {
}
Label::~Label() {
}
void Label::Draw() const {
  Font &font = Font::GetInstance();

  Font::VertexListType vertex_list;
  Font::IndexListType index_list;

  for (size_t i = 0 ; i < text.size() ; i++) {
    unsigned char ch = text[i];
    float tex_left, tex_right, tex_top, tex_bottom;
    font.GetCharacterTextureQuad(ch, &tex_left, &tex_right, &tex_top, &tex_bottom);

    float left, right, top, bottom;
    left = i * kFontSize;
    right = left + kFontSize;
    top = 0;
    bottom = top - kFontSize;

    // 3 2
    // 0 1
    runa::TextureVertex v0;
    v0.vertex[0] = left;
    v0.vertex[1] = bottom;
    v0.vertex[2] = 0;
    v0.texcoord[0] = tex_left;
    v0.texcoord[1] = tex_bottom;

    runa::TextureVertex v1;
    v1.vertex[0] = right;
    v1.vertex[1] = bottom;
    v1.vertex[2] = 0;
    v1.texcoord[0] = tex_right;
    v1.texcoord[1] = tex_bottom;

    runa::TextureVertex v2;
    v2.vertex[0] = right;
    v2.vertex[1] = top;
    v2.vertex[2] = 0;
    v2.texcoord[0] = tex_right;
    v2.texcoord[1] = tex_top;

    runa::TextureVertex v3;
    v3.vertex[0] = left;
    v3.vertex[1] = top;
    v3.vertex[2] = 0;
    v3.texcoord[0] = tex_left;
    v3.texcoord[1] = tex_top;

    int base_index = vertex_list.size();

    vertex_list.push_back(v0);
    vertex_list.push_back(v1);
    vertex_list.push_back(v2);
    vertex_list.push_back(v3);

    index_list.push_back(base_index + 0);
    index_list.push_back(base_index + 1);
    index_list.push_back(base_index + 2);

    index_list.push_back(base_index + 0);
    index_list.push_back(base_index + 2);
    index_list.push_back(base_index + 3);
  }

  // 적절히 그리기
  BasicTextureShader &shader = BasicTextureShader::GetInstance();
  shader.Use();

  font.BindFontTexture();

  // apply projection
  matsu::mat4 projection;
  int win_w = runa::Window::GetInstance().width();
  int win_h = runa::Window::GetInstance().height();
  projection = matsu::Matrix::Ortho<float>(0, win_w, 0, win_h, -10.0f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(position.x(), win_h - position.y(), 0);
  projection *= matsu::Matrix::Scale<float>(scale, scale, 1);
  shader.SetMatrix(projection.Pointer());

  //색 설정
  std::vector<runa::Color4ub> color_list(vertex_list.size(), color);
  GLuint color_location = shader.color_location();
  glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, color_list[0].data);
  glEnableVertexAttribArray(color_location);

  GLint position_location = shader.position_location();
  glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(runa::TextureVertex), &vertex_list[0].vertex[0]);
  glEnableVertexAttribArray(position_location);

  GLint texcoord_location = shader.texcoord_location();
  glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(runa::TextureVertex), &vertex_list[0].texcoord[0]);
  glEnableVertexAttribArray(texcoord_location);

  glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);

  SR_ASSERT(runa::GLTool::CheckError("draw label"));
}
void Label::DrawBorder(const matsu::vec4 &color) const {
  float width = (kFontSize * text.size()) * scale;
  float height = kFontSize* scale;
  float x = position.x();
  float y = position.y();
  matsu::Rectf rect(x-1, y+1, width+2, height+2);
  runa::UIDrawHelper::DrawLineRect(rect, color);
}

}