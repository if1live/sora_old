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
#ifndef NADEKO_BOARD_H_
#define NADEKO_BOARD_H_

#include <list>
#include "matsu/vector.h"
#include "matsu/matsu_enum.h"

//opengl
#include "sora/platform.h"
#if SR_WIN
#include <GL/glew.h>
#include <GL/glfw.h>
#endif

#include "aki/aki_enum.h"

namespace nadeko {;

class Board;
class Player;

class Board {
public:
  Board(int win_size, int win_height, int tile_size);
  ~Board();

  void Draw() const;
  void DrawTile(int x, int y, const matsu::vec4 &color) const;
  void DrawGrid() const;
  void DrawApple() const;
  
  int GetBoardWidth() const;
  int GetBoardHeight() const;

  // 판을 나갔는가?
  bool IsOverBoard(int x, int y) const;

  // 사과의 위치
  const matsu::ivec2 &get_apple_position() const;
  void CreateApple(const Player &player);

private:
  int tile_size_;
  int win_width_;
  int win_height_;
  matsu::ivec2 apple_position_;
  aki::TexturePtr texture_;
};

class Player {
public:
  typedef std::list<matsu::ivec2> PositionListType;
  typedef PositionListType::const_iterator ConstIterator;
  typedef PositionListType::iterator Iterator;

public:
  Player(int x, int y, matsu::Direction2 dir);
  ~Player();
  void Push(const matsu::ivec2 &pos);
  void Draw(const Board &board);
  void SetNextDirection(matsu::Direction2 direction);
  const matsu::ivec2 &GetHead() const;
  void Move(Board *board);

  ConstIterator Begin() const;
  ConstIterator End() const;

private:
  std::list<matsu::ivec2> pos_list_;
  int length_;
  matsu::Direction2 direction_;
  matsu::Direction2 next_direction_;
};
}
#endif  // NADEKO_BOARD_H_
