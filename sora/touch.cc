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
#include "sora/touch.h"

#define USE_TOUCH_LOG 0
#if USE_TOUCH_LOG
#define TOUCH_LOG(...)  LOGI(__VA_ARGS__)
#else
#define TOUCH_LOG(...)
#endif

namespace sora {;
TouchDevice::TouchDevice()
: prev_pos_count(0),
curr_pos_count(0) {
  memset(prev_evt_list_, 0, sizeof(prev_evt_list_));
  memset(curr_evt_list_, 0, sizeof(curr_evt_list_));
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
  memset(curr_evt_list_, 0, sizeof(curr_evt_list_));
  int index = 0;
  SR_ASSERT(began_evt_list_.size() + moved_evt_list_.size() <= 5);
  BOOST_FOREACH(const TouchEvent &evt, began_evt_list_) {
    curr_evt_list_[index] = evt;
    index++;
  }
  BOOST_FOREACH(const TouchEvent &evt, moved_evt_list_) {
    curr_evt_list_[index] = evt;
    index++;
  }
  BOOST_FOREACH(const TouchEvent &evt, ended_evt_list_) {
    curr_evt_list_[index] = evt;
    index++;
  }
  BOOST_FOREACH(const TouchEvent &evt, cancelled_evt_list_) {
    curr_evt_list_[index] = evt;
    index++;
  }

  //occur event
  if (!cancelled_evt_list_.empty()) {
    for (int i = 0 ; i < listener_list_.size() ; i++) {
      TouchListener *l = listener_list_[i];
      l->TouchCancelled(cancelled_evt_list_);
    }
  }
  if(!ended_evt_list_.empty()) {
    for (int i = 0 ; i < listener_list_.size() ; i++) {
      TouchListener *l = listener_list_[i];
      l->TouchEnded(ended_evt_list_);
    }
  }
  if(!moved_evt_list_.empty()) {
    for (int i = 0 ; i < listener_list_.size() ; i++) {
      TouchListener *l = listener_list_[i];
      l->TouchMoved(moved_evt_list_);
    }
  }
  if(!began_evt_list_.empty()) {
    for (int i = 0 ; i < listener_list_.size() ; i++) {
      TouchListener *l = listener_list_[i];
      l->TouchBegan(began_evt_list_);
    }
  }

  cancelled_evt_list_.clear();
  moved_evt_list_.clear();
  began_evt_list_.clear();
  ended_evt_list_.clear();

  curr_pos_count = index;
  prev_pos_count = curr_pos_count;

  memcpy(prev_evt_list_, curr_evt_list_, sizeof(curr_evt_list_));
}
void TouchDevice::PrintLog() {
  if (curr_pos_count != 0) {
    std::ostringstream oss;
    oss << "CurrPos:";
    for (int i = 0 ; i < curr_pos_count ; i++) {
      const TouchEvent &evt = curr_evt_list_[i];
      switch(evt.evt_type) {
      case kTouchBegan:
        oss << "b";
        break;
      case kTouchMoved:
        oss << "m";
        break;
      case kTouchEnded:
        oss << "e";
        break;
      case kTouchCancelled:
        oss << "c";
        break;
      }
      oss << "(" << evt.curr_x << "," << evt.curr_y << ") ";
    }
    std::string msg = oss.str();
    TOUCH_LOG("%s", msg.c_str());
  }
}

void TouchDevice::ClearEventList() {
  began_evt_list_.clear();
  moved_evt_list_.clear();
  ended_evt_list_.clear();
  cancelled_evt_list_.clear();
}

void TouchDevice::AddListener(TouchListener *listener) {
  using namespace std;
  vector<TouchListener*>::iterator found = find(listener_list_.begin(),
    listener_list_.end(), listener);
  if (found == listener_list_.end()) {
    listener_list_.push_back(listener);
  }
}
void TouchDevice::RemoveListener(TouchListener *listener) {
  using namespace std;
  vector<TouchListener*>::iterator found = find(listener_list_.begin(),
    listener_list_.end(), listener);
  if (found != listener_list_.end()) {
    listener_list_.erase(found);
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