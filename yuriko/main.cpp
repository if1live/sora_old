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
#include "sora/timer.h"

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
  // init glew
  SORA_init_gl_env(); //glewInit();
  Timer_Init();

	//logic begin
  SORA_setup_graphics(kWinWidth, kWinHeight);

  
  float prev_time = Timer_GetSecond();
  while(true) {
    SORA_draw_frame();
    float curr_time = Timer_GetSecond();
    float dt = curr_time - prev_time;
    SORA_update_frame(dt);

    glfwSwapBuffers();
    prev_time = curr_time;
    Timer_Tick();
  }
  //logic end

  glfwTerminate();
	return 0;
}