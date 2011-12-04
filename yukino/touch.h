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
#ifndef YUKINO_TOUCH_H_
#define YUKINO_TOUCH_H_

#include "sora/template_library.h"

namespace yukino {;
typedef enum {
  kTouchBegan,
  kTouchMoved,
  kTouchEnded,
  kTouchCancelled,
} TouchEventType;

struct TouchEvent {
  int prev_x;
  int prev_y;
  int curr_x;
  int curr_y;
  TouchEventType evt_type;
};

class TouchDevice : public sora::Singleton<TouchDevice> {
public:
  TouchDevice();
  ~TouchDevice();

  void AddBeganEvent(const TouchEvent &evt);
  void AddMovedEvent(const TouchEvent &evt);
  void AddEndedEvent(const TouchEvent &evt);
  void AddCancelledEvent(const TouchEvent &evt);

  void Update();
  void PrintLog();

  // 멀티터치가 5개 이상 인식될 일은 없고
  // 마찬가지로 
  int curr_pos_list[5][2];
  int prev_pos_list[5][2];
  int curr_pos_count;
  int prev_pos_count;

private:
  typedef std::vector<TouchEvent> TouchEventListType;
  TouchEventListType began_evt_list_;
  TouchEventListType moved_evt_list_;
  TouchEventListType ended_evt_list_;
  TouchEventListType cancelled_evt_list_;
};

#if SR_WIN
// glfw기반으로 구현된것
void glfwMouseUpdate(int win_width, int win_height);
#endif
}

#endif  // YUKINO_TOUCH_H_