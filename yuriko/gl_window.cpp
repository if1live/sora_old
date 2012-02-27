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
#include "yuriko_stdafx.h"
#include "gl_window.h"
#include "sora/render/gl_helper.h"

namespace yuriko {;
GLWindow::GLWindow()
  : width_(-1), height_(-1), content_scale_(1), win_mode_(kWinModeWindow) {}

GLWindow::~GLWindow() {
}

void GLWindow::Init() {
  using sora::GLHelper;

  SR_ASSERT(width_ > 0);
  SR_ASSERT(height_ > 0);
#if SR_WIN
  // Initialize GLFW
  if( !glfwInit() ) {
    exit( EXIT_FAILURE );
  }
  // Open an OpenGL window
  int screen_mode = 0;
  if (win_mode_ == kWinModeFullscreen) {
    screen_mode = GLFW_FULLSCREEN;
  } else {
    screen_mode = GLFW_WINDOW;
  }
  if( !glfwOpenWindow(width_, height_, 0,0,0,0,0,0, screen_mode ) ) {
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
#endif
  // gl 관련 정보 출력
  LOGI("GLVersion:%s\n", GLHelper::GetVersion().c_str());
  LOGI("GLVender:%s\n", GLHelper::GetVender().c_str());
  LOGI("GLRenderer:%s\n", GLHelper::GetRenderer().c_str());
  using std::vector;
  using std::string;
  LOGI("GLExtension:");
  const vector<string> &ext_list = GLHelper::GetExtensionList();
  for (size_t i = 0 ; i < ext_list.size() ; i++) {
    LOGI("%s ", ext_list[i].c_str());
  }
}
void GLWindow::Deinit() {
  glfwTerminate();
}
void GLWindow::SwapBuffer() {
  glfwSwapBuffers();
}
}