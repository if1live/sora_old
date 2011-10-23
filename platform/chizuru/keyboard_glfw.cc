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
#include "chizuru/keyboard.h"
#include "sora/platform.h"
#if SR_WIN
#include <GL/glfw.h>

namespace chizuru {;
Keyboard::Keyboard() {
}
Keyboard::~Keyboard() {
}
void Keyboard::Update() {
}
KeyState Keyboard::GetKeyState(int key) {
  int state;
  switch(key) {
  case KeyIdentifierSpace:
    state = glfwGetKey(GLFW_KEY_SPACE);
    break;
  case KeyIdentifierEsc:
    state = glfwGetKey(GLFW_KEY_ESC);
    break;
  case KeyIdentifierUp:
    state = glfwGetKey(GLFW_KEY_UP);
    break;
  case KeyIdentifierDown:
    state = glfwGetKey(GLFW_KEY_DOWN);
    break;
  case KeyIdentifierLeft:
    state = glfwGetKey(GLFW_KEY_LEFT);
    break;
  case KeyIdentifierRight:
    state = glfwGetKey(GLFW_KEY_RIGHT);
    break;
  default:
    state = glfwGetKey(key);
    break;
  }
  if (state == GLFW_RELEASE) {
    return KeyStateRelease;
  } else if(state == GLFW_PRESS) {
    return KeyStatePress;
  } else {
    SR_ASSERT(!"not valid");
    return KeyStateRelease;
  }
}
bool Keyboard::IsPressed(int key) {
  return (KeyStatePress == GetKeyState(key));
}
bool Keyboard::IsReleased(int key) {
  return (KeyStateRelease == GetKeyState(key));
}
}
#endif