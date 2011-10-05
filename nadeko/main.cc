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
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <string>
#include <memory>

#include "matsu/matrix.h"
#include "matsu/vector.h"

#include "runa/shader.h"
#include "runa/shader_program.h"
#include "runa/window.h"

#include "mio/path.h"

#include "aki/image.h"
#include "aki/image_loader.h"

#include "board.h"

const int kTileSize = 16;
const float kMoveDelay = 0.2f;

void Update(float dt);
void Draw();
void Init();

using matsu::Matrix;
using matsu::vec4;
using std::string;
using std::auto_ptr;
using runa::ShaderProgram;
using runa::Window;
using nadeko::Board;
using nadeko::Player;

using matsu::kDirection2Down;
using matsu::kDirection2Up;
using matsu::kDirection2Left;
using matsu::kDirection2Right;
using mio::Path;

auto_ptr<Board> board;
auto_ptr<Player> player;
bool show_bg = false;

int main() {
  // load sample texture
  string bg_file = "testdata/nadeko/nadeko_01.jpg";
  bg_file = Path::AppPath(bg_file);
  aki::ImagePtr img = aki::ImageLoader::Load(bg_file);
  const int width = img->desc().width();
  const int height = img->desc().height();

  // const int width = 480;
  // const int height = 320;
  Window &win = Window::GetInstance();
  win.Initialize(width, height);

  Init();

  // Main loop
  int running = GL_TRUE;
  while (running) {
    static double prev_time = glfwGetTime();
    double curr_time = glfwGetTime();
    float dt = static_cast<float>(curr_time - prev_time);
    // OpenGL rendering goes here...
    Update(dt);
    Draw();

    // Swap front and back rendering buffers
    glfwSwapBuffers();
    // Check if ESC key was pressed or window was closed
    running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

    prev_time = curr_time;
  }

  // Close window and terminate GLFW
  win.Deinitialize();
  
  // Exit program
  exit(EXIT_SUCCESS);
}

void Init() {
  Window &win = Window::GetInstance();
  board.reset(new Board(win.get_width(), win.get_height(), kTileSize));
  player.reset(new Player(board->GetBoardWidth() / 2, 
    board->GetBoardHeight() /2, matsu::kDirection2Left));

  board->CreateApple(*player);
}

void Update(float dt) {
  // check key
  if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
    player->SetNextDirection(kDirection2Left);
  } else if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
    player->SetNextDirection(kDirection2Right);
  } else if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
    player->SetNextDirection(kDirection2Up);
  } else if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
    player->SetNextDirection(kDirection2Down);
  }

  if (glfwGetKey('B') == GLFW_PRESS) {
    show_bg = true;
  } else {
    show_bg = false;
  }

  const float move_delay = kMoveDelay;
  static float remain_time = move_delay;
  remain_time -= dt;
  if(remain_time < 0) {
    remain_time = move_delay;
    player->Move(board.get());
  }
}
void Draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  if (show_bg == true) {
    board->DrawBackground();
  }
  player->Draw(*board);

  board->DrawApple();
  board->DrawGrid();
}