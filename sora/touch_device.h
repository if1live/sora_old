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
#ifndef SORA_TOUCH_DEVICE_H_
#define SORA_TOUCH_DEVICE_H_

#include "touch_event.h"

#if SR_WIN

namespace sora {;

class TouchDevice {
public:
  TouchDevice() {}
  virtual ~TouchDevice() {}

  virtual void UpdateState(int state, int xpos, int ypos, int tick_count, float timestamp) = 0;
  virtual const std::vector<TouchEvent> &GetCreatedEvent() const = 0;

  // 실행 환경에 따라서 적절한 장치를 리턴한다
  static TouchDevice &GetInstance();
};


class TouchDevice_glfw : public TouchDevice {
public:
  TouchDevice_glfw();
  ~TouchDevice_glfw();

  //현재 상태의 정보를 넘기는 함수. 매 프레임마다 해당 함수를 호출한다
	void UpdateState(int state, int xpos, int ypos, int tick_count, float timestamp);
  const std::vector<TouchEvent> &GetCreatedEvent() const { return fired_evt_list_; }

protected:
	int prev_state_;
	int prev_x_;
	int prev_y_;
	TouchEvent last_touch_;

  std::vector<TouchEvent> fired_evt_list_;
};
}

#endif  // SR_WIN

#endif  // SORA_TOUCH_DEVICE_H_