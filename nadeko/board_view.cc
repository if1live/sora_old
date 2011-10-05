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
#include "nadeko/board_view.h"

#include "runa/basic_color_shader.h"
#include "runa/basic_texture_shader.h"
#include "runa/shader_program.h"
#include "matsu/matrix.h"
#include "matsu/vector.h"
#include "aki/image_loader.h"
#include "aki/texture.h"
#include "aki/image.h"
#include "mio/path.h"

#include "nadeko/board.h"

using runa::BasicColorShader;
using runa::BasicTextureShader;
using std::string;
using std::vector;
using matsu::vec2;
using matsu::vec4;
using matsu::ivec2;

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
  bg_file = mio::Path::AppPath(bg_file);
  aki::ImagePtr img = aki::ImageLoader::Load(bg_file);
  texture_.reset(new aki::Texture());
  texture_->LoadImage(img.get());

  // body texture test shader
  const char vertex_src[] = "uniform mat4 u_mvp;  "
    "attribute vec4 a_position; "
    "attribute vec2 a_texcoord; "
    "uniform vec4 u_color;  "    
    "varying vec4 v_color;  "
    "varying vec2 v_texcoord; "
    "void main()  "
    "{  "
    "v_texcoord = a_texcoord; "
    "v_color = u_color; "
    "gl_Position = u_mvp * a_position;"
    "}";
  const char fragment_src[] = ""
      "precision mediump float;  "
      "varying vec4 v_color;  "
      "varying vec2 v_texcoord; "
      "uniform sampler2D s_texture; "
      "void main()  "
      "{  "
      "vec4 tmp = texture2D(s_texture, v_texcoord); "
      "vec4 tmp1 = vec4(tmp.x * v_color.x, tmp.y * v_color.y, tmp.z * v_color.z, tmp.w * v_color.w);"
      "float minimum = 0.3;"
      "float r = tmp1.x > minimum ? tmp1.x : minimum; "
      "float g = tmp1.y > minimum ? tmp1.y : minimum; "
      "float b = tmp1.z > minimum ? tmp1.z : minimum; "
      "float a = tmp1.w > minimum ? tmp1.w : minimum; "
      "gl_FragColor = vec4(r, g, b, a);  "
      "}  ";
  body_shader_prog.reset(new runa::ShaderProgram(vertex_src, fragment_src));
}
BoardView::~BoardView() {
}

void BoardView::DrawBackground() const {
  BasicTextureShader &shader = BasicTextureShader::GetInstance();

  GLint position_location = shader.position_location();
  GLint texcoord_location = shader.texcoord_location();

  glEnable(GL_TEXTURE_2D);
  shader.Use();

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0, 1, 0, 1, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  shader.SetMatrix(projection.Pointer());

  //색 설정
  matsu::vec4 color(0.2, 0.2, 0.2, 0.2);
  shader.SetColor4fv(color.Pointer());

  //3 2
  //0 1
  int vertex[] = {
    0, 0,
    1, 0,
    1, 1,
    0, 1,
  };
  glVertexAttribPointer(position_location, 2, GL_INT, GL_FALSE, 0, vertex);
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
  glDisable(GL_TEXTURE_2D);
}

void BoardView::DrawGrid() const {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  GLint position_location = shader.position_location();
  GLint color_location = shader.color_location();
  GLint mvp_location = shader.mvp_location();
  
  shader.Use();

  //set color
  matsu::vec4 gray(0.5, 0.5, 0.5, 1.0);
  glUniform4fv(color_location, 1, gray.Pointer());

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0.0f, 
    (float)win_width_, 0.0f, (float)win_height_, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  projection *= matsu::Matrix::Scale<float>(tile_size_, tile_size_, 1);
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, projection.Pointer());

  // 세로줄 
  int board_width = win_width_ / tile_size_;
  int board_height = win_height_ / tile_size_;
  vector<matsu::vec2> grid_line_;
  for (int i = 0 ; i < board_width ; i++) {
    grid_line_.push_back(vec2(i, 0));
    grid_line_.push_back(vec2(i, board_height));
  }
  // 가로줄
  for (int i = 0 ; i < board_height ; i++) {
    grid_line_.push_back(vec2(0, i));
    grid_line_.push_back(vec2(board_width, i));
  }
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, grid_line_[0].Pointer());
  glEnableVertexAttribArray(position_location);
  glDrawArrays(GL_LINES, 0, grid_line_.size());
}

void BoardView::DrawColorTile(int x, int y, const matsu::vec4 &color) const {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  GLint position_location = shader.position_location();
  
  shader.Use();

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0.0f, 
    (float)win_width_, 0.0f, (float)win_height_, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  projection *= matsu::Matrix::Scale<float>(tile_size_, tile_size_, 1);
  shader.SetMatrix(projection.Pointer());

  //색 설정
  shader.SetColor4fv(color.Pointer());

  //3 2
  //0 1
  int vertex[] = {
    x, y ,
    (x + 1), y,
    (x + 1), (y + 1),
    x, (y + 1)
  };
  glVertexAttribPointer(position_location, 2, GL_INT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  //index써서 그리기
  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3,
  };
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, index);
}
void BoardView::DrawBodyTile(int x, int y, const matsu::vec4 &color) const {
  GLint position_location = body_shader_prog->GetAttribLocation("a_position");
  GLint texcoord_location = body_shader_prog->GetAttribLocation("a_texcoord");
  GLint color_location = body_shader_prog->GetUniformLocation("u_color");
  GLint mvp_location = body_shader_prog->GetUniformLocation("u_mvp");
  SR_ASSERT(position_location != -1);
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(color_location != -1);
  SR_ASSERT(mvp_location != -1);


  glEnable(GL_TEXTURE_2D);
  body_shader_prog->Use();

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0.0f, 
    (float)win_width_, 0.0f, (float)win_height_, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  projection *= matsu::Matrix::Scale<float>(tile_size_, tile_size_, 1);
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, projection.Pointer());

  //색 설정
  glUniform4fv(color_location, 1, color.Pointer());

  //3 2
  //0 1
  int vertex[] = {
    x, y ,
    (x + 1), y,
    (x + 1), (y + 1),
    x, (y + 1)
  };
  glVertexAttribPointer(position_location, 2, GL_INT, GL_FALSE, 0, vertex);
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
  glDisable(GL_TEXTURE_2D);
}
void BoardView::DrawApple(int x, int y) const {
  matsu::vec4 color(0, 0, 1, 1);
  DrawColorTile(x, y, color);
}
void BoardView::DrawPlayer(const Player &player) const {
  Player::ConstIterator it = player.Begin();
  Player::ConstIterator endit = player.End();

  for ( ; it != endit ; it++) {
    const ivec2 &pos = *it;
    if (it == player.Begin()) {
      //첫번째는 머리
      vec4 head_color(1, 0, 0, 1);
      DrawColorTile(pos.x(), pos.y(), head_color);
    } else {
      //body
      //vec4 body_color(0, 1, 0, 1);
      vec4 body_color(0.9, 0.9, 0.9, 0.8);
      DrawBodyTile(pos.x(), pos.y(), body_color);
    }
  }
}
}
