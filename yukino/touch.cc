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
#include "yukino/touch.h"

#define USE_TOUCH_LOG 1
#if USE_TOUCH_LOG
#define TOUCH_LOG(...)  LOGI(__VA_ARGS__)
#else
#define TOUCH_LOG(...)
#endif

namespace yukino {;
TouchDevice::TouchDevice()
  : prev_pos_count(0),
  curr_pos_count(0) {
  memset(prev_pos_list, 0, sizeof(int) * 5 * 2);
  memset(curr_pos_list, 0, sizeof(int) * 5 * 2);
}
TouchDevice::~TouchDevice() {
}
void TouchDevice::AddBeganEvent(const TouchEvent &evt) {
  TOUCH_LOG("Began : %d %d", evt.curr_x, evt.curr_y);
  began_evt_list_.push_back(evt);
}
void TouchDevice::AddMovedEvent(const TouchEvent &evt) {
  TOUCH_LOG("Moved : %d %d", evt.curr_x, evt.curr_y);
  moved_evt_list_.push_back(evt);
}
void TouchDevice::AddEndedEvent(const TouchEvent &evt) {
  TOUCH_LOG("Ended : %d %d", evt.curr_x, evt.curr_y);
  ended_evt_list_.push_back(evt);
}
void TouchDevice::AddCancelledEvent(const TouchEvent &evt) {
  TOUCH_LOG("Cancel: %d %d", evt.curr_x, evt.curr_y);
  cancelled_evt_list_.push_back(evt);
}
void TouchDevice::Update() {
  memset(curr_pos_list, 0, sizeof(int) * 5 * 2);
  int index = 0;
  SR_ASSERT(began_evt_list_.size() + moved_evt_list_.size() <= 5);
  BOOST_FOREACH(const TouchEvent &evt, began_evt_list_) {
    curr_pos_list[index][0] = evt.curr_x;
    curr_pos_list[index][1] = evt.curr_y;
    index++;
  }
  BOOST_FOREACH(const TouchEvent &evt, moved_evt_list_) {
    curr_pos_list[index][0] = evt.curr_x;
    curr_pos_list[index][1] = evt.curr_y;
    index++;
  }

  cancelled_evt_list_.clear();
  moved_evt_list_.clear();
  began_evt_list_.clear();

  curr_pos_count = index;
  prev_pos_count = curr_pos_count;
  memcpy(prev_pos_list, curr_pos_list, sizeof(int) * 5 * 2);
}
void TouchDevice::PrintLog() {
  if (curr_pos_count != 0) {
    std::ostringstream oss;
    oss << "CurrPos:";
    for(int i = 0 ; i < curr_pos_count ; i++) {
      oss << "(curr_pos_list[i][0]" << "," << curr_pos_list[i][1] << ") ";
    }
    std::string msg = oss.str();
    TOUCH_LOG("%s", msg.c_str());
  }
}

#if SR_WIN
void glfwMouseUpdate(int win_width, int win_height) {
  //터치 흉내에는 마우스 왼쪽만 있어도 충분하다
  static int prev_x = 0;
  static int prev_y = 0;
  static int prev_state = GLFW_RELEASE;
  static bool cancelled_occur = false;

  int curr_x, curr_y;
  glfwGetMousePos(&curr_x, &curr_y);
  int curr_state = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);

  bool curr_out = false;
  if (curr_x < 0 || curr_x > win_width || curr_y < 0 || curr_y > win_height) {
    curr_out = true;
  }
  bool prev_out = false;
  if (prev_x < 0 || prev_x > win_width || prev_y < 0 || prev_y > win_height) {
    prev_out = true;
  }

  if (curr_state == GLFW_PRESS && curr_out == true && prev_out == false) {
      // 마우스가 눌린상태로 화면 이탈
      TouchEvent evt;
      evt.prev_x = prev_x;
      evt.prev_y = prev_y;
      evt.curr_x = curr_x;
      evt.curr_y = curr_y;
      evt.evt_type = kTouchCancelled;
      TouchDevice::GetInstance().AddCancelledEvent(evt);
      cancelled_occur = true;
  }
  
  if (curr_out == false) {
    if (prev_state == GLFW_RELEASE && curr_state == GLFW_PRESS) {
      // began
      TouchEvent evt;
      evt.prev_x = curr_x;
      evt.prev_y = curr_y;
      evt.curr_x = curr_x;
      evt.curr_y = curr_y;
      evt.evt_type = kTouchBegan;
      TouchDevice::GetInstance().AddBeganEvent(evt);

      // 이전에 발생한 cancelled플래그를 began만 내릴수있다
      cancelled_occur = false;
    }
    if (prev_state == GLFW_PRESS && curr_state == GLFW_RELEASE && cancelled_occur == false) {
      TouchEvent evt;
      evt.prev_x = prev_x;
      evt.prev_y = prev_y;
      evt.curr_x = curr_x;
      evt.curr_y = curr_y;
      evt.evt_type = kTouchEnded;
      TouchDevice::GetInstance().AddEndedEvent(evt);

    }
    if (prev_state == GLFW_PRESS && curr_state == GLFW_PRESS && cancelled_occur == false) {
      if (!(prev_x == curr_x && prev_y == curr_y)) {
        TouchEvent evt;
        evt.prev_x = prev_x;
        evt.prev_y = prev_y;
        evt.curr_x = curr_x;
        evt.curr_y = curr_y;
        evt.evt_type = kTouchMoved;
        TouchDevice::GetInstance().AddMovedEvent(evt);
      }
    }
  }

  prev_x = curr_x;
  prev_y = curr_y;
  prev_state = curr_state;

  //터치 이벤트를 날렸으면 flush수행
  TouchDevice::GetInstance().Update();
}
#endif
}

#undef TOUCH_LOG
#undef USE_TOUCH_LOG