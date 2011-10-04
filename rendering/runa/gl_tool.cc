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
#include "precompile.h"
#include "runa/gl_tool.h"
#include "hayami/hayami.h"

using std::string;

namespace runa {;
bool GLTool::CheckError(const std::string &title) {
  int error = glGetError();
  if (error != GL_NO_ERROR) {
    string error_msg;
    switch (error) {
    case GL_INVALID_ENUM:
      error_msg = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error_msg = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error_msg = "GL_INVALID_OPERATION";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error_msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    case GL_OUT_OF_MEMORY:
      error_msg = "GL_OUT_OF_MEMORY";
      break;
    default:
      error_msg = "UNKNOWN";
      break;
    }
    hayami::ConsoleLogger logger(string("GL"));
    logger.Errorf("OpenGL Error [%s] : %s", title.c_str(), error_msg.c_str());
    return false;
  } else {
    return true;
  }
}
}
