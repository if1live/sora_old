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
  int key_code_table[][2] = {
    { kKeyIdentifierSpace, GLFW_KEY_SPACE },
    { kKeyIdentifierEsc, GLFW_KEY_ESC },
    { kKeyIdentifierUp, GLFW_KEY_UP },
    { kKeyIdentifierDown, GLFW_KEY_DOWN },
    { kKeyIdentifierLeft, GLFW_KEY_LEFT },
    { kKeyIdentifierRight, GLFW_KEY_RIGHT },
  };

  int state = 0;
  bool is_special_key_occur = false;
  int key_code_table_row = sizeof(key_code_table) / (sizeof(int) * 2);
  for (int i = 0 ; i < key_code_table_row ; i++) {
    if (key == key_code_table[i][0]) {
      state = glfwGetKey(key);
      is_special_key_occur = true;
      break;
    }
  }
  if (is_special_key_occur == false) {
    state = glfwGetKey(key);
  }
  
  if (state == GLFW_RELEASE) {
    return kButtonReleased;
  } else if(state == GLFW_PRESS) {
    return kButtonPressed;
  }
  SR_ASSERT(!"do not reach");
  return kButtonReleased;
}
bool Keyboard::IsPressed(int key) {
  return (kButtonPressed == GetButtonState(key));
}
bool Keyboard::IsReleased(int key) {
  return (kButtonReleased == GetButtonState(key));
}
}
#endif