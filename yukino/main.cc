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
#include "renderer.h"
#include "sora/gl_helper.h"
#include "sora/texture.h"

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

  // gl 관련 정보 출력
  printf("GLVersion:%s\n", sora::GLHelper::GetVersion().c_str());
  printf("GLVender:%s\n", sora::GLHelper::GetVender().c_str());
  printf("GLRenderer:%s\n", sora::GLHelper::GetRenderer().c_str());
  using std::vector;
  using std::string;
  printf("GLExtension:");
  const vector<string> &ext_list = sora::GLHelper::GetExtensionList();
  for (int i = 0 ; i < ext_list.size() ; i++) {
    printf("%s ", ext_list[i].c_str());
  }
  printf("\n");


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
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  srglEnable(GL_TEXTURE_2D);
  sora::Texture &tex = sora::Texture::Sample();
  srglBindTexture(GL_TEXTURE_2D, tex.handle);
  
  srglBegin(GL_QUADS);
  srglTexCoord2f(0, 0); srglVertex3f(-0.5, -0.5, 0);
  srglTexCoord2f(1, 0); srglVertex3f(0.5, -0.5, 0);
  srglTexCoord2f(1, 1); srglVertex3f(0.5, 0.5, 0);
  srglTexCoord2f(0, 1); srglVertex3f(-0.5, 0.5, 0);
  srglEnd();
  
  sora::GLHelper::CheckError("DrawEnd");
}
void Init() {
  yukino::Renderer::GetInstance().Init();

  srglViewport(0, 0, win_width, win_height);
}