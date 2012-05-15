/*  Copyright (C) 2011-2012 by if1live */
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
#ifndef SORA_KEYBOARD_EVENT_H_
#define SORA_KEYBOARD_EVENT_H_

#include "event_queue.h"

namespace sora {;
enum {
  kKeyboardPress,
  kKeyboardRelease,
};

struct KeyboardEvent {
  enum {
    kUp,
    kDown,
    kLeft,
    kRight,
  };
  KeyboardEvent() : state(-1), is_special_key(false), ch(0) { }
  int state;  //press/release
  bool is_special_key;  //화살표, esc, enter같은거용
  char ch;    //어느 키에 대해서?. 대문자로 강제 보정하자
};

typedef EventQueue<KeyboardEvent> KeyboardEventQueue;
}

#endif  // SORA_KEYBOARD_EVENT_H_