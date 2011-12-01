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
#include "yukino_stdafx.h"
#include "sora/clock.h"
#include "renderer.h"
#include "sora/gl_helper.h"

const int win_width = 480;
const int win_height = 320;

void Update(int ms);
void Draw(int ms);
void Init();

int main(int argc, char *argv) {
  // init glfw
  int running = true;
  // Initialize GLFW
  if( !glfwInit() ) {
    exit( EXIT_FAILURE );
  }
  // Open an OpenGL window
  if( !glfwOpenWindow( win_width,win_height, 0,0,0,0,0,0, GLFW_WINDOW ) ) {
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  // init glew(for GLES 2.0 support)
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


  Init();
  // Main loop
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
  }
  // Close window and terminate GLFW
  yukino::Renderer::DestroyInstance();
  glfwTerminate();  /////
  // Exit program
  exit( EXIT_SUCCESS );

  return 0;
}

void Update(int ms) {
}
void Draw(int ms) {
  // OpenGL rendering goes here...
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_TEXTURE_2D);
  
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, 0);
  glTexCoord2f(1, 0); glVertex3f(0.5, -0.5, 0);
  glTexCoord2f(1, 1); glVertex3f(0.5, 0.5, 0);
  glTexCoord2f(0, 1); glVertex3f(-0.5, 0.5, 0);
  glEnd();
  
  sora::GLHelper::CheckError("DrawEnd");
}
void Init() {
  yukino::Renderer::GetInstance().Init();

  glViewport(0, 0, win_width, win_height);
  
  //sample texture
  unsigned char texture_data[] = {
    255, 0, 0, 255,
    0, 255, 0, 255,
    0, 0, 255, 255,
    255, 255, 255, 255,
  };
  GLuint tex_id;
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
  
}