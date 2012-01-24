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
#include "yukino_stdafx.h"
#include "sora/clock.h"
#include "sora/render/gl_helper.h"
#include "sora/render/texture.h"
#include "sora/io/filesystem.h"
#include "sora/render/gl_window.h"
#include "sora/io/memory_file.h"

#include "sora/render/immediate_mode_emulator.h"
#include "sora/ui/touch.h"

#include "sora/render/texture_manager.h"
#include "sora/render/texture_helper.h"

#include "sora/render/scene_manager.h"
#include "sora/render/scene.h"

#include "game_scene.h"
#include "intro_scene.h"
#include "main_scene.h"

// 첫번쨰 타겟은 안드로이드로 생각하고 거기에 맞춰서 작업하자
//const int win_width = 960;
//const int win_height = 640;
const int win_width = 800;
const int win_height = 480;

void Update(int ms);
void Draw(int ms);
void Init();

void InitWindow(int w, int h);


using namespace sora;
using namespace yukino;

int main(int argc, char *argv) {
  // init glfw
  sora::GLWindow &win = GLWindow::GetInstance();
  win.SetSize(win_width, win_height);
  win.set_content_scale(1);
  win.Init();

  Init();

  //sora::TextureManagerThreadRunner texture_thd_runner;
  //boost::thread texture_thd(texture_thd_runner);

  /////////////
  // set scene
  //Scene *scene = new GameScene();
  //Scene *scene = new IntroScene();
  Scene *scene = new MainScene();
  SceneManager::GetInstance().Push(scene);
  ////////////

  // Main loop
  int running = true;
  int prev_runtime = sora::Clock::currtime;
  while( running ) {
    int curr_ms = sora::Clock::Tick();
    int delta_ms = curr_ms - prev_runtime;
    // update timer
    Update(delta_ms);
    Draw(delta_ms);

    // Swap front and back rendering buffers
    glfwSwapBuffers();
    // Check if ESC key was pressed or window was closed
    running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );

    prev_runtime = sora::Clock::currtime;

    //////////
    // engine logic
    sora::TextureManager::GetInstance().ProcessResponse();
    sora::TextureManager::GetInstance().ProcessRequest(); //single thd
  }

  //texture thread stop
  //sora::TextureManagerThreadRunner::run_thread = false;
  //texture_thd.join();

  // Close window and terminate GLFW
  sora::ImmediateModeEmulator::DestroyInstance();
  
  glfwTerminate();  /////
  // Exit program
  exit( EXIT_SUCCESS );

  return 0;
}

void Update(int ms) {
  using namespace sora;
#if SR_WIN
  // 윈도우의 경우, 마우스로 터치 흉내내기
  //sora::glfwMouseUpdate(win_width, win_height);
  //sora::TouchDevice::GetInstance().PrintLog();
#endif

  SceneManager::GetInstance().Update(ms);

  /*
#if SR_WIN
  if (glfwGetKey('T') == GLFW_PRESS) {
    // texture alloc info
    TextureManager &mgr = TextureManager::GetInstance();
    printf("used %d / unused %d\n", mgr.GetUsedHandleCount(), mgr.GetUnusedHandleCount());

    TextureManager::NameHandleDictType::iterator it = mgr.Begin();
    TextureManager::NameHandleDictType::iterator endit = mgr.End();
    for ( ; it != endit ; it++) {
      const std::string &name = it->first;
      const TextureHandle &handle = it->second;
      Texture *tex = mgr.GetTexture(handle);

      if (TextureHelper::IsSystemTexture(*tex) == false) {
        printf("%s\n", name.c_str());
      }
    }
    printf("=============\n");
  }
#endif
  */
}

void Draw(int ms) {  
  SceneManager::GetInstance().Draw();
  sora::GLHelper::CheckError("DrawEnd");
}

void Init() {
  sora::ImmediateModeEmulator::GetInstance().Init();
  srglViewport(0, 0, win_width, win_height);  
}
