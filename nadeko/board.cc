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
#include "board.h"
#include <ctime>
#include <algorithm>
#include <GL/glew.h>
#include "runa/shader_program.h"
#include "runa/basic_color_shader.h"
#include "matsu/matrix.h"
#include "mio/path.h"
#include "aki/image.h"
#include "aki/texture.h"
#include "aki/image_loader.h"

using aki::TexturePtr;
using aki::ImagePtr;
using mio::Path;
using std::list;
using std::vector;
using runa::BasicColorShader;
using std::string;
using matsu::ivec2;
using matsu::vec2;
using matsu::vec4;
using matsu::kDirection2Down;
using matsu::kDirection2Up;
using matsu::kDirection2Left;
using matsu::kDirection2Right;

namespace nadeko {;

std::auto_ptr<runa::ShaderProgram> shader_prog;

Board::Board(int win_width, int win_height, int tile_size) 
: tile_size_(tile_size),
win_width_(win_width),
win_height_(win_height),
apple_position_(-1, -1) {
  // create board draw shader
  BasicColorShader::GetInstance().Initialize();

  // reload texture
  string bg_file = "testdata/nadeko/nadeko_01.jpg";
  bg_file = Path::AppPath(bg_file);
  aki::ImagePtr img = aki::ImageLoader::Load(bg_file);
  texture_.reset(new aki::Texture());
  texture_->LoadImage(img.get());

  //texture test shader
  const char texture_vertex_src[] = "uniform mat4 u_mvp;  "
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
  const char texture_fragment_src[] = ""
      "precision mediump float;  "
      "varying vec4 v_color;  "
      "varying vec2 v_texcoord; "
      "uniform sampler2D s_texture; "
      "void main()  "
      "{  "
      "vec4 tmp = texture2D(s_texture, v_texcoord); "
      "vec4 tmp1 = vec4(tmp.x * v_color.x, tmp.y * v_color.y, tmp.z * v_color.z, tmp.w * v_color.w);"
      "gl_FragColor = tmp1;  "
      "}  ";
  shader_prog.reset(new runa::ShaderProgram(texture_vertex_src,
    texture_fragment_src));
  shader_prog->Link();
  shader_prog->Validate();
}
Board::~Board() {
}
void Board::Draw() const {
}
void Board::DrawColorTile(int x, int y, const matsu::vec4 &color) const {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  GLint position_location = shader.get_position_location();
  
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
void Board::DrawTextureTile(int x, int y, const matsu::vec4 &color) const {
  GLint position_location = shader_prog->GetAttribLocation("a_position");
  GLint texcoord_location = shader_prog->GetAttribLocation("a_texcoord");
  GLint color_location = shader_prog->GetUniformLocation("u_color");
  GLint mvp_location = shader_prog->GetUniformLocation("u_mvp");
  SR_ASSERT(position_location != -1);
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(color_location != -1);
  SR_ASSERT(mvp_location != -1);


  glEnable(GL_TEXTURE_2D);
  shader_prog->Use();

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
  float tex_tile_width = 1.0f / GetBoardWidth();
  float tex_tile_height = 1.0f / GetBoardHeight();
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
void Board::DrawGrid() const {
  BasicColorShader &shader = BasicColorShader::GetInstance();
  GLint position_location = shader.get_position_location();
  GLint color_location = shader.get_color_location();
  GLint mvp_location = shader.get_mvp_location();
  
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
  vector<matsu::vec2> grid_line_;
  for (int i = 0 ; i < GetBoardWidth() ; i++) {
    grid_line_.push_back(vec2(i, 0));
    grid_line_.push_back(vec2(i, GetBoardHeight()));
  }
  // 가로줄
  for (int i = 0 ; i < GetBoardHeight() ; i++) {
    grid_line_.push_back(vec2(0, i));
    grid_line_.push_back(vec2(GetBoardWidth(), i));
  }
  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, grid_line_[0].Pointer());
  glEnableVertexAttribArray(position_location);
  glDrawArrays(GL_LINES, 0, grid_line_.size());
}
int Board::GetBoardWidth() const {
  return win_width_ / tile_size_;
}
int Board::GetBoardHeight() const {
  return win_height_ / tile_size_;
}
bool Board::IsOverBoard(int x, int y) const {
  if (x < 0 || x >= GetBoardWidth()) {
    return true;
  }
  if (y < 0 || y >= GetBoardHeight()) {
    return true;
  }
  return false;
}
const matsu::ivec2 &Board::get_apple_position() const {
  return apple_position_;
}
void Board::CreateApple(const Player &player) {
  vector<ivec2> board_point_list;
  board_point_list.reserve(GetBoardWidth() * GetBoardHeight());
  for (int i = 0 ; i < GetBoardWidth() ; i++) {
    for (int j = 0 ; j < GetBoardHeight() ; j++) {
      ivec2 pos(i, j);
      board_point_list.push_back(pos);
    }
  }

  //player위치를 제거
  Player::ConstIterator it = player.Begin();
  Player::ConstIterator endit = player.End();
  for ( ; it != endit ; it++) {
    const ivec2 &invalid_pos = *it;

    vector<ivec2>::iterator board_it = board_point_list.begin();
    vector<ivec2>::iterator board_endit = board_point_list.end();
    vector<ivec2>::iterator found = std::find(board_it, board_endit, invalid_pos);
    board_point_list.erase(found);
  }

  //적당히 1개 뽑기
  srand((unsigned int)(time(NULL)));
  std::random_shuffle(board_point_list.begin(), board_point_list.end());
  apple_position_ = board_point_list[0];
}
void Board::DrawApple() const {
  vec4 color(0, 0, 1, 1);
  const ivec2 &pos = get_apple_position();
  DrawColorTile(pos.x(), pos.y(), color);
}
void Board::DrawBackground() const {
  GLint position_location = shader_prog->GetAttribLocation("a_position");
  GLint texcoord_location = shader_prog->GetAttribLocation("a_texcoord");
  GLint color_location = shader_prog->GetUniformLocation("u_color");
  GLint mvp_location = shader_prog->GetUniformLocation("u_mvp");
  SR_ASSERT(position_location != -1);
  SR_ASSERT(texcoord_location != -1);
  SR_ASSERT(color_location != -1);
  SR_ASSERT(mvp_location != -1);

  glEnable(GL_TEXTURE_2D);
  shader_prog->Use();

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0, 1, 0, 1, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, projection.Pointer());

  //색 설정
  vec4 color(0.2, 0.2, 0.2, 0.2);
  glUniform4fv(color_location, 1, color.Pointer());

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
//////////////////////////////////////

Player::Player(int x, int y, matsu::Direction2 dir) 
: length_(10), 
direction_(dir),
next_direction_(dir) {
  pos_list_.push_back(matsu::ivec2(x, y));
}
Player::~Player() {
}
void Player::Push(const matsu::ivec2 &pos) {
  //머리가 가장 앞
  pos_list_.push_front(pos);
  if (pos_list_.size() > length_) {
    pos_list_.pop_back();
  }
}
void Player::Draw(const Board &board) {
  // head
  list<ivec2>::iterator it = pos_list_.begin();
  const ivec2 &head_pos = *it;
  vec4 head_color(1, 0, 0, 1);
  board.DrawColorTile(head_pos.x(), head_pos.y(), head_color);
  
  // body
  it++; //body start
  list<ivec2>::iterator endit = pos_list_.end();
  for( ; it != endit ; it++) {
    //vec4 body_color(0, 1, 0, 1);
    vec4 body_color(0.9, 0.9, 0.9, 0.8);
    const ivec2 &body_pos = *it;
    board.DrawTextureTile(body_pos.x(), body_pos.y(), body_color);
  }
}
const matsu::ivec2 &Player::GetHead() const {
  return pos_list_.front();
}
void Player::SetNextDirection(matsu::Direction2 direction) {
  if (direction_ == matsu::kDirection2Down
    || direction_ == matsu::kDirection2Up) {
    if (direction == matsu::kDirection2Left 
      || direction == matsu::kDirection2Right) {
      next_direction_ = direction;
    }
  }
  if (direction_ == kDirection2Left
    || direction_ == kDirection2Right) {
    if (direction == kDirection2Down 
      || direction == kDirection2Up) {
      next_direction_ = direction;
    }
  }
}
void Player::Move(Board *board) {
  direction_ = next_direction_;

  const ivec2 &head_pos = GetHead();
  ivec2 next_head_pos = head_pos;
  if (direction_ == kDirection2Down) {
    next_head_pos += ivec2(0, -1);
  } else if (direction_ == kDirection2Up) {
    next_head_pos += ivec2(0, +1);
  } else if (direction_ == kDirection2Left) {
    next_head_pos += ivec2(-1, 0);
  } else if (direction_ == kDirection2Right) {
    next_head_pos += ivec2(+1, 0);
  }

  /// next로 이동하면 맵을 넘어가는가?
  if (board->IsOverBoard(next_head_pos.x(), next_head_pos.y())) {
    exit(-1);
  }

  /// next로 이동시 죽는지 확인하기
  list<ivec2>::iterator it = std::find(pos_list_.begin(),
    pos_list_.end(), next_head_pos);
  if (it != pos_list_.end()) {
    //몸통과 충돌한다!
    exit(-1);
  }

  /// next에 사과가 있는지 확인하기
  if (board->get_apple_position() == next_head_pos) {
    length_++;
    board->CreateApple(*this);
  }
  Push(next_head_pos);
}
Player::ConstIterator Player::Begin() const {
  return pos_list_.begin();
}
Player::ConstIterator Player::End() const {
  return pos_list_.end();
}
}
