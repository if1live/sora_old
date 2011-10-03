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
#include "board.h"
#include <ctime>
#include <algorithm>
#include <GL/glew.h>
#include "runa/shader_program.h"
#include "matsu/matrix.h"

using std::list;
using std::vector;
using runa::ShaderProgram;
using std::string;
using matsu::ivec2;
using matsu::vec2;
using matsu::vec4;
using matsu::kDirection2Down;
using matsu::kDirection2Up;
using matsu::kDirection2Left;
using matsu::kDirection2Right;


namespace nadeko {;
Board::Board(int win_width, int win_height, int tile_size) 
: tile_size_(tile_size),
win_width_(win_width),
win_height_(win_height),
apple_position_(-1, -1) {
  // create board draw shader
  string vertex_src = "uniform mat4 u_mvp;  "
    "attribute vec4 a_position;  "
    "uniform vec4 u_color;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "v_color = u_color; "
    "gl_Position = u_mvp * a_position;"
    "}";

  string fragment_src = ""
    "precision mediump float;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "gl_FragColor = v_color;  "
    "}  ";
  shader_program_.reset(new ShaderProgram(vertex_src, fragment_src));
}
Board::~Board() {
}
void Board::Draw() const {
}
void Board::DrawTile(int x, int y, const matsu::vec4 &color) const {
  GLint position_location = shader_program_->GetAttribLocation("a_position");
  SR_ASSERT(position_location != -1);
  GLint color_location = shader_program_->GetUniformLocation("u_color");
  SR_ASSERT(color_location != -1);
  GLint mvp_location = shader_program_->GetUniformLocation("u_mvp");
  SR_ASSERT(mvp_location != -1);
  
  shader_program_->Use();

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0.0f, 
    (float)win_width_, 0.0f, (float)win_height_, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, 1);
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

  //index써서 그리기
  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3,
  };
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, index);
}
void Board::DrawGrid() const {
  GLint position_location = shader_program_->GetAttribLocation("a_position");
  SR_ASSERT(position_location != -1);
  GLint color_location = shader_program_->GetUniformLocation("u_color");
  SR_ASSERT(color_location != -1);
  GLint mvp_location = shader_program_->GetUniformLocation("u_mvp");
  SR_ASSERT(mvp_location != -1);

  shader_program_->Use();

  //set color
  matsu::vec4 gray(0.5, 0.5, 0.5, 1.0);
  glUniform4fv(color_location, 1, gray.Pointer());

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0.0f, 
    (float)win_width_, 0.0f, (float)win_height_, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, 1);
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
  DrawTile(pos.x(), pos.y(), color);
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
  board.DrawTile(head_pos.x(), head_pos.y(), head_color);
  
  // body
  it++; //body start
  list<ivec2>::iterator endit = pos_list_.end();
  for( ; it != endit ; it++) {
    vec4 body_color(0, 1, 0, 1);
    const ivec2 &body_pos = *it;
    board.DrawTile(body_pos.x(), body_pos.y(), body_color);
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
