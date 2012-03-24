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
#include "touch_device.h"

namespace sora {;

TouchDevice &TouchDevice::GetInstance() {
  static TouchDevice_glfw device;
  return device;
}

static int next_touch_id = 1;

TouchDevice_glfw::TouchDevice_glfw()
: prev_state_(kInputRelease), 
prev_x_(0),
prev_y_(0) {
  last_touch_.evt_type = kTouchEnded;
}
TouchDevice_glfw::~TouchDevice_glfw()
{
}

void TouchDevice_glfw::UpdateState(int state, int xpos, int ypos, int tick_count, float timestamp)
{
  fired_evt_list_.clear();

	//올바르지 않으 위치로 이동한 경우(마우스 터치후 불가능한 영역
	//화면밖으로 드래그한 경우에는 터치를 종료시킨다...는 일단 지웠지만 나중에 추가 검토
	//어차피 마우스로는 싱글터치밖에 안되니까 판정을 복잡하게 늘릴 필요가 없다
	if(prev_state_ == kInputRelease && state == kInputPress) {
		//touch began
		SR_ASSERT(last_touch_.evt_type == kTouchEnded 
			|| last_touch_.evt_type == kTouchCancelled);
		TouchEvent touch;
		touch.x = xpos;
		touch.y = ypos;
		touch.prev_x = xpos;
		touch.prev_y = ypos;
		touch.timestamp = timestamp;
    touch.tick_count = tick_count;
		touch.evt_type = kTouchBegan;
		touch.uid = next_touch_id;
		last_touch_ = touch;
		
    //event fire!
    fired_evt_list_.push_back(touch);

	} else if(prev_state_ == kInputPress && state == kInputPress) {
		if(xpos != prev_x_ || ypos != prev_y_) {
			//touch moved
			SR_ASSERT(last_touch_.evt_type == kTouchBegan 
				|| last_touch_.evt_type == kTouchMoved);
			TouchEvent touch;
			touch.x = xpos;
			touch.y = ypos;
			touch.prev_x = prev_x_;
			touch.prev_y = prev_y_;
      touch.tick_count = tick_count;
      touch.timestamp = timestamp;
			touch.evt_type = kTouchMoved;
			touch.uid = next_touch_id;

      //터치 이벤트 Fire!
      fired_evt_list_.push_back(touch);

			last_touch_ = touch;
		}
	} else if(prev_state_ == kInputPress && state == kInputRelease) {
		//touch end
		SR_ASSERT(last_touch_.evt_type == kTouchBegan
			|| last_touch_.evt_type == kTouchMoved);
		TouchEvent touch;
		touch.x = xpos;
		touch.y = ypos;
		touch.prev_x = prev_x_;
		touch.prev_y = prev_y_;
    touch.timestamp = timestamp;
    touch.tick_count = tick_count;
		touch.evt_type = kTouchEnded;
		touch.uid = next_touch_id;

		last_touch_ = touch;
		
    //touch evt fire!
    fired_evt_list_.push_back(touch);

		//touch id교체
		next_touch_id++;

	}

	//end
	prev_state_ = state;
	prev_x_ = xpos;
	prev_y_ = ypos;
}


}