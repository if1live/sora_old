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

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <cstdio>

#include "sora/sora_main.h"
#include "sora/sys/device.h"
#include "sora/core/timer.h"
#include "sora/shadow_map_main.h"
//#include "sora/test_function.h"
#include "sora/selection_main.h"
#include "sora/celshading_main.h"

const int kWinWidth = 480;
const int kWinHeight = 800;
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
  //while(true) {
  //  SORA_test_draw(kWinWidth, kWinHeight);
  //  glfwSwapBuffers();
  //}

  //gl2 test
  //SORA_init_gl_env(); //glewInit();
  //while(true) {
  //  SORA_test_draw2(kWinWidth, kWinHeight);
  //  glfwSwapBuffers();
  //}

  // init glew
  SORA_init_gl_env(); //glewInit();
  Timer_Init();

  sora::Device device;

  /*
	//logic begin
  SORA_setup_graphics(&device, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    SORA_draw_frame(&device);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    SORA_update_frame(&device, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      exit(0);
    }
  }
  */
  
  /*
  //depth map test
  ShadowMap_setup_graphics(&device, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    ShadowMap_draw_frame(&device);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    ShadowMap_update_frame(&device, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      exit(0);
    }
  }
  */
  /*
  //selection test
  Selection_setup_graphics(&device, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    Selection_draw_frame(&device);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    Selection_update_frame(&device, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      exit(0);
    }
  }
  */
  //celshading test
  sora::celshading::CelShading_setup_graphics(&device, kWinWidth, kWinHeight);
  float prev_time = Timer_GetSecond();
  while(true) {
    sora::celshading::CelShading_draw_frame(&device);
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    sora::celshading::CelShading_update_frame(&device, dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
      exit(0);
    }
  }

  //logic end

  glfwTerminate();
	return 0;
}