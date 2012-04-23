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

#include "sora/renderer/gl/gl_inc.h"
#include <cstdlib>
#include <cstdio>
#include <functional>

#include "sora/event/keyboard_event.h"

#include "sora/sora_main.h"
#include "sora/sys/device.h"
#include "sora/core/timer.h"
#include "sora/shadow_map_main.h"
#include "sora/test_function.h"
#include "sora/selection_main.h"
#include "sora/celshading_main.h"
#include "sora/depth_map_main.h"

#include "sora/freeglut_main.h"
#include "sora/sysfont_main.h"

#if SR_WIN && (SR_GLES == 0)

//const int kWinWidth = 480;
//const int kWinHeight = 800;
const int kWinWidth = 640;
const int kWinHeight = 480;

typedef std::function<void(sora::Device*,int, int)> InitFunctionType;
typedef std::function<void(sora::Device*)> DrawFunctionType;
typedef std::function<void(sora::Device*, float)> UpdateFunctionType;

//간단함 함수객체 기반의 테스트 함수
void run_sample(sora::Device *dev, InitFunctionType init_func, DrawFunctionType draw_func, UpdateFunctionType update_func) {
  //selection test
  init_func(dev, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    draw_func(dev);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    update_func(dev, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      break;
    }
  }
}

void run_freeglutfont(sora::Device *dev) {
  InitFunctionType init_func = sora::freeglut::setup_graphics;
  DrawFunctionType draw_func = sora::freeglut::draw_frame;
  UpdateFunctionType update_func = sora::freeglut::update_frame;
  run_sample(dev, init_func, draw_func, update_func);
}

void run_sysfont(sora::Device *dev) {
  InitFunctionType init_func = sora::sysfont::setup_graphics;
  DrawFunctionType draw_func = sora::sysfont::draw_frame;
  UpdateFunctionType update_func = sora::sysfont::update_frame;
  run_sample(dev, init_func, draw_func, update_func);
}

/*
void run_selection(sora::Device *dev) {
  //selection test
  sora::selection::setup_graphics(dev, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    sora::selection::draw_frame(dev);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    sora::selection::update_frame(dev, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      break;
    }
  }
}

void run_shadow_map(sora::Device *dev) {
  //depth map test
  sora::shadowmap::setup_graphics(dev, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    sora::shadowmap::draw_frame(dev);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    sora::shadowmap::update_frame(dev, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      exit(0);
    }
  }
}

void run_celshading(sora::Device *dev) {
  //celshading test
  sora::celshading::setup_graphics(dev, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    sora::celshading::draw_frame(dev);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    sora::celshading::update_frame(dev, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      break;
    }
  }
}

void run_depthmap(sora::Device *dev) {
  //celshading test
  sora::depthmap::setup_graphics(dev, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    sora::depthmap::draw_frame(dev);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    sora::depthmap::update_frame(dev, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      break;
    }
  }
}

void run_gles_1_test() {
  while(true) {
    SORA_test_draw(kWinWidth, kWinHeight);
    glfwSwapBuffers();
  }
}

void run_gles_2_test() {
  SORA_init_gl_env(); //glewInit();
  while(true) {
    SORA_test_draw2(kWinWidth, kWinHeight);
    glfwSwapBuffers();
  }
}
*/
int main(int argc, char *argv[]) {
  if( !glfwInit() ) {
    exit( EXIT_FAILURE );
  }
  // Open an OpenGL window
  if( !glfwOpenWindow( kWinWidth,kWinHeight, 0,0,0,0,0,0, GLFW_WINDOW ) ) {
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  //gl test
  //run_gles_1_test();
  //run_gles_2_test();

  // init glew
  glewInit();
  Timer_Init();

  sora::Device device;

  /*
	//logic begin
  SORA_setup_graphics(&device, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    //glfw 키보드 이벤트 처기
    if(glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
      sora::KeyboardEvent evt;
      evt.is_special_key = true;
      evt.ch = sora::KeyboardEvent::kUp;
      device.keyboard_evt_queue().Push(evt);
    }
    if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
      sora::KeyboardEvent evt;
      evt.is_special_key = true;
      evt.ch = sora::KeyboardEvent::kDown;
      device.keyboard_evt_queue().Push(evt);
    }
    if(glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
      sora::KeyboardEvent evt;
      evt.is_special_key = true;
      evt.ch = sora::KeyboardEvent::kLeft;
      device.keyboard_evt_queue().Push(evt);
    }
    if(glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
      sora::KeyboardEvent evt;
      evt.is_special_key = true;
      evt.ch = sora::KeyboardEvent::kRight;
      device.keyboard_evt_queue().Push(evt);
    }

    SORA_draw_frame(&device);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    SORA_update_frame(&device, dt);
    device.EndTick();

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      exit(0);
    }
  }
  */
  
  //logic end
  //run_freeglutfont(&device);
  run_sysfont(&device);
  //run_selection(&device);
  //run_depthmap(&device);
  //run_shadow_map(&device);

  glfwTerminate();
	return 0;
}
#endif
