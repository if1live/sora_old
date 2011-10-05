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
Board::Board(int win_width, int win_height, int tile_size) 
: tile_size_(tile_size),
win_width_(win_width),
win_height_(win_height),
apple_position_(-1, -1) {
  // create board draw shader
  BasicColorShader::GetInstance().Initialize();
}
Board::~Board() {
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
const matsu::ivec2 &Board::apple_position() const {
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

//////////////////////////////////////

Player::Player(int x, int y, matsu::Direction2 dir) 
  : initial_x_(x), initial_y_(y), initial_dir_(dir) {
  Reset();
}
Player::~Player() {
}
void Player::Reset() {
  length_ = 10;
  direction_ = initial_dir_;
  next_direction_ = initial_dir_;
  alive_ = true;
  pos_list_.clear();
  pos_list_.push_back(matsu::ivec2(initial_x_, initial_y_));
}
void Player::Push(const matsu::ivec2 &pos) {
  //머리가 가장 앞
  pos_list_.push_front(pos);
  if (pos_list_.size() > length_) {
    pos_list_.pop_back();
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
  if (alive_ == false) {
    // 죽었으면 로직을 돌릴 이유가 없다
    return;
  }

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
    alive_ = false;
  }

  /// next로 이동시 죽는지 확인하기
  list<ivec2>::iterator it = std::find(pos_list_.begin(),
    pos_list_.end(), next_head_pos);
  if (it != pos_list_.end()) {
    //몸통과 충돌한다!
    alive_ = false;
  }

  /// next에 사과가 있는지 확인하기
  if (board->apple_position() == next_head_pos) {
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
