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
#include "sora/gl_helper.h"
#include "sora/texture.h"
#include "sora/filesystem.h"

#include "renderer.h"
#include "touch.h"

#include "glassless3d.h"

const int win_width = 960;
const int win_height = 640;

void Update(int ms);
void Draw(int ms);
void Init();

void InitWindow(int w, int h);

int main(int argc, char *argv) {
  // init glfw
  InitWindow(win_width, win_height);
  Init();

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
  }
  // Close window and terminate GLFW
  yukino::Renderer::DestroyInstance();
  glfwTerminate();  /////
  // Exit program
  exit( EXIT_SUCCESS );

  return 0;
}

sora::Texture tex;
void Update(int ms) {
  yukino::glfwMouseUpdate(win_width, win_height);

  yukino::Glassless3d::GetInstance().update(ms * 0.001);
}
void Draw(int ms) {
  // OpenGL rendering goes here...
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  yukino::Glassless3d::GetInstance().draw();
  /*
  //sora::Texture &tex = sora::Texture::Sample();
  srglBindTexture(GL_TEXTURE_2D, tex.handle);

  //set matrix
  srglMatrixMode(SR_PROJECTION);
  srglLoadIdentity();
  srglPerspective(60.0f, 480.0f / 320.0f, 0.1f, 1000.0f);
  srglLookAt(0.0f, 0.0f, 4.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f);

  srglMatrixMode(SR_MODELVIEW);
  srglLoadIdentity();
  srglScalef(0.5, 0.5, 0.5);

  ////////////////////////////
  srglBegin(GL_QUADS);
  srglTexCoord2f(0, 0); srglVertex3f(-0.5, -0.5, 0);
  srglTexCoord2f(1, 0); srglVertex3f(0.5, -0.5, 0);
  srglTexCoord2f(1, 1); srglVertex3f(0.5, 0.5, 0);
  srglTexCoord2f(0, 1); srglVertex3f(-0.5, 0.5, 0);
  srglEnd();
  */
  sora::GLHelper::CheckError("DrawEnd");
}
void Init() {
  yukino::Renderer::GetInstance().Init();

  srglViewport(0, 0, win_width, win_height);

  using std::string;
  string filename = "\\res\\Scene10.png";
  //string filename = "\\res\\test.png";
  filename = sora::Filesystem::GetAppPath(filename);
  sora::Texture::LoadFromPNG(filename, &tex);

  yukino::Glassless3d::GetInstance().init();
}

void InitWindow(int w, int h) {
  // Initialize GLFW
  if( !glfwInit() ) {
    exit( EXIT_FAILURE );
  }
  // Open an OpenGL window
  if( !glfwOpenWindow(w, h, 0,0,0,0,0,0, GLFW_WINDOW ) ) {
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

}