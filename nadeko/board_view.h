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
#ifndef NADEKO_BOARD_VIEW_H_
#define NADEKO_BOARD_VIEW_H_

//opengl
#include "sora/platform.h"
#include "sora/tr1_include.h"
#if SR_WIN
#include <GL/glew.h>
#include <GL/glfw.h>
#endif
#include "aki/aki_enum.h"
#include "matsu/vector.h"
#include "runa/runa_enum.h"

namespace nadeko {;
class Player;

class BoardView {
 public:
  BoardView(int win_width, int win_height, int tile_size);
  ~BoardView();

  void DrawBackground() const;
  void DrawGrid() const;
  void DrawColorTile(int x, int y, const matsu::vec4 &color) const;
  void DrawBodyTile(int x, int y, const matsu::vec4 &color) const;
  void DrawApple(int x, int y) const;
  void DrawPlayer(const Player &player) const;
 private:
  int tile_size_;
  int win_width_;
  int win_height_;
  aki::TexturePtr texture_;

  //몸통을 그릴때 사용하는 쉐이더
  std::auto_ptr<runa::ShaderProgram> body_shader_prog;
};
}

#endif  // NADEKO_BOARD_VIEW_H_
