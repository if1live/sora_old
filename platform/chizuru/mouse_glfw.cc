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
#include "chizuru/mouse_glfw.h"
#include <gl/glfw.h>
#include "chizuru/chizuru_enum.h"
#include "chizuru/touch_event.h"
#include "chizuru/touch_device.h"

namespace chizuru {;
MouseGlfw::MouseGlfw()
: prev_x_(0),
prev_y_(0),
prev_state_(GLFW_RELEASE) {
}
MouseGlfw::~MouseGlfw() {
}
void MouseGlfw::Update() {
  /// 아이폰의 터치와 1:1로 통하는것만 구현할 예정이다
  /// 그래서 왼쪽에 대해서만 일단 처리한다
  int curr_state = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
  int curr_x, curr_y;
  glfwGetMousePos(&curr_x, &curr_y);

  TouchEventType type = kTouchEventInvalid;
  if (prev_state_ == GLFW_RELEASE && curr_state == GLFW_RELEASE) {
    ; //안눌려져있는상태에서 안누른 경우, 아무것도 없다
  } else if (prev_state_ == GLFW_RELEASE && curr_state == GLFW_PRESS) {
    type = kTouchEventBegan;
    next_id_++;

  } else if (prev_state_ == GLFW_PRESS && curr_state == GLFW_RELEASE) {
    type = kTouchEventEnded;

  } else if (prev_state_ == GLFW_PRESS && curr_state == GLFW_PRESS) {
    if (prev_x_ != curr_x || prev_y_ != curr_y) {
      type = kTouchEventMoved;
    }
  }

  TouchEvent touch_evt;   // empty;
  if (type != kTouchEventInvalid) {
    touch_evt.id = next_id_;
    touch_evt.curr_x = curr_x;
    touch_evt.curr_y = curr_y;
    touch_evt.prev_x = prev_x_;
    touch_evt.prev_y = prev_y_;
    touch_evt.evt_type = type;

    std::vector<TouchEvent> evt_list;
    evt_list.push_back(touch_evt);
    
    if (type == kTouchEventBegan) {
      TouchDevice::GetInstance().TouchBegan(evt_list);
    } else if (type == kTouchEventMoved) {
      TouchDevice::GetInstance().TouchMoved(evt_list);
    } else if (type == kTouchEventEnded) {
      TouchDevice::GetInstance().TouchEnded(evt_list);
    } else if (type == kTouchEventCancelled) {
      TouchDevice::GetInstance().TouchCancelled(evt_list);
    }
  }

  prev_state_ = curr_state;
  prev_x_ = curr_x;
  prev_y_ = curr_y;
}
int MouseGlfw::next_id_ = 0;
}
