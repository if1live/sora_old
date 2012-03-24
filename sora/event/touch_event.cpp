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
#include "sora_stdafx.h"
#include "touch_event.h"

namespace sora {;

TouchEventQueue::TouchEventQueue(bool save_history)
  : save_history_(save_history) {
}
TouchEventQueue::~TouchEventQueue() {
}

const TouchEvent &TouchEventQueue::Front() const {
  if(IsEmpty()) {
    //return fake event
    static TouchEvent fake_evt;
    static bool create = false;
    if(create == false) {
      create = true;

      fake_evt.evt_type = -1;
      fake_evt.x = 0;
      fake_evt.y = 0;
      fake_evt.prev_x = 0;
      fake_evt.prev_y = 0;
      fake_evt.tick_count = -1;
      fake_evt.timestamp = 0;
      fake_evt.uid = 0;
    }
    return fake_evt;
  } else {
    return evt_queue_.front();
  }
}
TouchEvent TouchEventQueue::Get() {
  TouchEvent evt = Front();
  if(evt_queue_.empty() == false) {
    evt_queue_.pop();
  }
  return evt;
}
void TouchEventQueue::Push(const TouchEvent &evt) {
  evt_queue_.push(evt);
  if(save_history_) {
    evt_history_.push_back(evt);
  }
}
bool TouchEventQueue::IsEmpty() const {
  return evt_queue_.empty();
}
}
