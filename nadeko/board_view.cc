﻿/*  Copyright (C) 2011 by if1live */
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
#include "nadeko/board_view.h"

#include "runa/basic_color_shader.h"
#include "runa/basic_texture_shader.h"
#include "runa/shader_program.h"
#include "runa/gl_tool.h"
#include "runa/matrix_stack.h"
#include "runa/color.h"
#include "matsu/matrix.h"
#include "matsu/vector.h"
#include "aki/image_loader.h"
#include "aki/texture2d.h"
#include "aki/texture_loader.h"
#include "aki/image.h"
#include "mio/path.h"
#include "mio/reader.h"

#include "nadeko/board.h"

using runa::BasicColorShader;
using runa::BasicTextureShader;
using runa::Color4ub;
using std::string;
using std::vector;
using matsu::vec2;
using matsu::vec4;
using matsu::ivec2;
using mio::Path;
using matsu::Matrix;
using runa::MatrixStack;

namespace nadeko {;
BoardView::BoardView(int win_width, int win_height, int tile_size)
: win_width_(win_width),
win_height_(win_height),
tile_size_(tile_size) {
  //사용할 쉐이더 초기화
  if (BasicColorShader::GetInstance().IsInitialized() == false) {
    BasicColorShader::GetInstance().Initialize();
  }
  if (BasicTextureShader::GetInstance().IsInitialized() == false) {
    BasicTextureShader::GetInstance().Initialize();
  }

  // reload texture
  string bg_file = "testdata/nadeko/nadeko_01.jpg";
  bg_file = Path::AppPath(bg_file);
  texture_.reset(new aki::Texture2D());
  GLuint tex_id;
  aki::TextureSize tex_size;
  aki::TextureLoader::LoadTexture2D(bg_file, &tex_id, &tex_size);
  texture_->SetTexture(tex_id, tex_size);

  // body texture test shader
  string vertex_file = Path::AppPath("testdata/nadeko/snake_body.vsh");
  string fragment_file = Path::AppPath("testdata/nadeko/snake_body.fsh");
  string vertex_src;
  string fragment_src;
  mio::reader::Read(vertex_file, &vertex_src);
  mio::reader::Read(fragment_file, &fragment_src);
  body_shader_prog.reset(new runa::ShaderProgram(vertex_src, fragment_src));
  body_shader_prog->Validate();

  SR_ASSERT(runa::GLTool::CheckError("snaek body shader"));
}
BoardView::~BoardView() {
}

void BoardView::DrawBackground() const {
  BasicTextureShader &shader = BasicTextureShader::GetInstance();

  GLint position_location = shader.position_location();
  GLint texcoord_location = shader.texcoord_location();

  shader.Use();
  texture_->Bind();

  // apply projection
  runa::MatrixStack projection_stack;
  projection_stack.MultiplyMatrix(matsu::Matrix::Ortho<float>(0, 1, 0, 1, -10.0f, 10.0f));
  shader.SetMatrix(projection_stack.Pointer());

  //색 설정
  Color4ub color(100, 100, 100, 100);
  vector<Color4ub> color_list(4, color);
  GLint color_location = shader.color_location();
  glEnableVertexAttribArray(color_location);
  glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, color_list[0].data);

  //3 2
  //0 1
  float vertex[] = {
    0, 0,
    1, 0,
    1, 1,
    0, 1,
  };
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  //텍스쳐 설정
  float texcoord[] = {
    0, 1,
    1, 1,
    1, 0,
    0, 0
  };
  glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
  glEnableVertexAttribArray(texcoord_location);

  //index써서 그리기
  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3,
  };
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, index);

  SR_ASSERT(runa::GLTool::CheckError("draw background"));
}

void BoardView::DrawColorTile(int x, int y, const runa::Color4ub &color) const {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  GLint position_location = shader.position_location();
  
  shader.Use();

  // apply projection
  MatrixStack projection_stack;
  projection_stack.MultiplyMatrix(Matrix::Ortho<float>(0.0f, 
    win_width_, 0.0f, win_height_, -10.0f, 10.0f));
  projection_stack.Scale(tile_size_, tile_size_, 1);
  shader.SetMatrix(projection_stack.Pointer());

  //색 설정
  vector<Color4ub> color_list(4, color);
  GLint color_location = shader.color_location();
  glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, color_list[0].data);
  glEnableVertexAttribArray(color_location);

  //3 2
  //0 1
  float vertex[] = {
    x, y ,
    (x + 1), y,
    (x + 1), (y + 1),
    x, (y + 1)
  };
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  //index써서 그리기
  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3,
  };
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, index);

  SR_ASSERT(runa::GLTool::CheckError("draw color tile"));
}
void BoardView::DrawBodyTile(int x, int y, const runa::Color4ub &color) const {
  GLint position_location = body_shader_prog->GetAttribLocation("a_position");
  GLint texcoord_location = body_shader_prog->GetAttribLocation("a_texcoord");
  GLint color_location = body_shader_prog->GetAttribLocation("a_color");
  GLint mvp_location = body_shader_prog->GetUniformLocation("u_mvp");
  SR_ASSERT(position_location != -1);
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(color_location != -1);
  SR_ASSERT(mvp_location != -1);


  body_shader_prog->Use();
  texture_->Bind();

  // apply projection
  MatrixStack projection_stack;
  projection_stack.MultiplyMatrix(Matrix::Ortho<float>(0.0f, 
    (float)win_width_, 0.0f, (float)win_height_, -10.0f, 10.0f));
  projection_stack.Scale(tile_size_, tile_size_, 1);
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, projection_stack.Pointer());

  //색 설정
  vector<Color4ub> color_list(4, color);
  glVertexAttribPointer(color_location, color.Dimension, GL_UNSIGNED_BYTE, GL_TRUE, 0, color_list[0].data);
  glEnableVertexAttribArray(color_location);

  //3 2
  //0 1
  float vertex[] = {
    x, y ,
    (x + 1), y,
    (x + 1), (y + 1),
    x, (y + 1)
  };
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  //텍스쳐 설정
  float board_width = win_width_ / tile_size_;
  float board_height = win_height_ / tile_size_;
  float tex_tile_width = 1.0f / board_width;
  float tex_tile_height = 1.0f / board_height;
  float texcoord[] = {
    x * tex_tile_width, -(y * tex_tile_height),
    (x+1) * tex_tile_width, -(y * tex_tile_height),
    (x+1) * tex_tile_width, -((y+1) * tex_tile_height),
    x * tex_tile_width, -((y+1) * tex_tile_height)
  };
  glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
  glEnableVertexAttribArray(texcoord_location);

  //index써서 그리기
  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3,
  };
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, index);

  SR_ASSERT(runa::GLTool::CheckError("draw body tile"));
}
void BoardView::DrawApple(int x, int y) const {
  const Color4ub &color = Color4ub::Blue();
  DrawColorTile(x, y, color);

  SR_ASSERT(runa::GLTool::CheckError("draw apple"));
}
void BoardView::DrawPlayer(const Player &player) const {
  Player::ConstIterator it = player.Begin();
  Player::ConstIterator endit = player.End();

  for ( ; it != endit ; it++) {
    const ivec2 &pos = *it;
    if (it == player.Begin()) {
      //첫번째는 머리
      const Color4ub &head_color = Color4ub::Red();
      DrawColorTile(pos.x(), pos.y(), head_color);
    } else {
      //body
      //vec4 body_color(0, 1, 0, 1);
      //Color4ub body_color(150, 150, 150, 150);
      Color4ub body_color(255, 255, 255, 255);
      DrawBodyTile(pos.x(), pos.y(), body_color);
    }
  }

  SR_ASSERT(runa::GLTool::CheckError("draw player"));
}
}
