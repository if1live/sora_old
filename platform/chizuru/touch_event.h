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
#ifndef PLATFORM_CHIZURU_TOUCH_EVENT_H_
#define PLATFORM_CHIZURU_TOUCH_EVENT_H_

#include "chizuru/chizuru_enum.h"

namespace chizuru {;
/// @bried ui좌표계 기준
struct TouchEvent {
  static TouchEvent Began(int id, int curr_x, int curr_y, int prev_x, int prev_y) {
    TouchEvent evt;
    evt.id = id;
    evt.curr_x = curr_x;
    evt.curr_y = curr_y;
    evt.prev_x = prev_x;
    evt.prev_y = prev_y;
    evt.evt_type = kTouchEventBegan;
  }
  static TouchEvent Moved(int id, int curr_x, int curr_y, int prev_x, int prev_y) {
    TouchEvent evt;
    evt.id = id;
    evt.curr_x = curr_x;
    evt.curr_y = curr_y;
    evt.prev_x = prev_x;
    evt.prev_y = prev_y;
    evt.evt_type = kTouchEventMoved;
  }
  static TouchEvent Ended(int id, int curr_x, int curr_y, int prev_x, int prev_y) {
    TouchEvent evt;
    evt.id = id;
    evt.curr_x = curr_x;
    evt.curr_y = curr_y;
    evt.prev_x = prev_x;
    evt.prev_y = prev_y;
    evt.evt_type = kTouchEventEnded;
  }
  static TouchEvent Cancelled(int id, int curr_x, int curr_y, int prev_x, int prev_y) {
    TouchEvent evt;
    evt.id = id;
    evt.curr_x = curr_x;
    evt.curr_y = curr_y;
    evt.prev_x = prev_x;
    evt.prev_y = prev_y;
    evt.evt_type = kTouchEventCancelled;
  }

  TouchEvent()
    : id(-1),
    curr_x(0),
    curr_y(0),
    prev_x(0),
    prev_y(0),
    evt_type(kTouchEventInvalid) {
  }

  int id;
  int curr_x;
  int curr_y;
  int prev_x;
  int prev_y;
  TouchEventType evt_type;
};
}

#endif  // PLATFORM_CHIZURU_TOUCH_EVENT_H_