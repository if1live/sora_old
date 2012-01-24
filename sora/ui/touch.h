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
#ifndef SORA_TOUCH_H_
#define SORA_TOUCH_H_

#include "sora/template_library.h"
#include "sora/input/input_enum.h"

#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;

struct TouchEvent;
class TouchDevice;
class TouchListener;

struct TouchEvent {
  int prev_x;
  int prev_y;
  int curr_x;
  int curr_y;
  TouchEventType evt_type;
};

class TouchDevice : public sora::Singleton<TouchDevice> {
public:
  typedef std::vector<TouchEvent> EventListType;
  TouchDevice();

  void AddBeganEvent(const TouchEvent &evt);
  void AddMovedEvent(const TouchEvent &evt);
  void AddEndedEvent(const TouchEvent &evt);
  void AddCancelledEvent(const TouchEvent &evt);

  void Update();
  void PrintLog();

  void ClearEventList();
  void AddListener(TouchListener *listener);
  void RemoveListener(TouchListener *listener);

private:
  ~TouchDevice();

  EventListType began_evt_list_;
  EventListType moved_evt_list_;
  EventListType ended_evt_list_;
  EventListType cancelled_evt_list_;

  // 멀티터치가 5개 이상 인식될 일은 없으니까 고정
  TouchEvent prev_evt_list_[5];
  TouchEvent curr_evt_list_[5];
  int curr_pos_count;
  int prev_pos_count;

  std::vector<TouchListener*> listener_list_;
};

class TouchListener {
public:
  virtual void TouchBegan(const TouchDevice::EventListType &evt_list) {}
  virtual void TouchMoved(const TouchDevice::EventListType &evt_list) {}
  virtual void TouchEnded(const TouchDevice::EventListType &evt_list) {}
  virtual void TouchCancelled(const TouchDevice::EventListType &evt_list) {}
};

#if SR_WIN
// glfw기반으로 구현된것
void glfwMouseUpdate(int win_width, int win_height);
#endif
}

#endif  // SORA_TOUCH_H_