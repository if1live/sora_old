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
#include "sora_stdafx.h"
#include "input_enum.h"
#include "keyboard.h"
#if SR_WIN
//#include <GL/glfw.h>

namespace sora {;
Keyboard::Keyboard() {
}
Keyboard::~Keyboard() {
}
void Keyboard::Update() {
}
ButtonState Keyboard::GetButtonState(int key) {
  int state;
  switch(key) {
  case kKeyIdentifierSpace:
    state = glfwGetKey(GLFW_KEY_SPACE);
    break;
  case kKeyIdentifierEsc:
    state = glfwGetKey(GLFW_KEY_ESC);
    break;
  case kKeyIdentifierUp:
    state = glfwGetKey(GLFW_KEY_UP);
    break;
  case kKeyIdentifierDown:
    state = glfwGetKey(GLFW_KEY_DOWN);
    break;
  case kKeyIdentifierLeft:
    state = glfwGetKey(GLFW_KEY_LEFT);
    break;
  case kKeyIdentifierRight:
    state = glfwGetKey(GLFW_KEY_RIGHT);
    break;
  default:
    state = glfwGetKey(key);
    break;
  }
  if (state == GLFW_RELEASE) {
    return kButtonReleased;
  } else if(state == GLFW_PRESS) {
    return kButtonPressed;
  } else {
    SR_ASSERT(!"not valid");
    return kButtonReleased;
  }
}
bool Keyboard::IsPressed(int key) {
  return (kButtonPressed == GetButtonState(key));
}
bool Keyboard::IsReleased(int key) {
  return (kButtonReleased == GetButtonState(key));
}
}
#endif