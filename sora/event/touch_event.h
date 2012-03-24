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
#ifndef SORA_TOUCH_EVENT_H_
#define SORA_TOUCH_EVENT_H_

#if SR_USE_PCH == 0
#include <vector>
#include <queue>
#endif

namespace sora {;

struct TouchEvent;
class TouchEventQueue;

enum {
  kTouchBegan,
  kTouchMoved,
  kTouchEnded,
  kTouchCancelled
};

enum {
	kInputRelease,
  kInputPress,
};

struct TouchEvent {
  int evt_type; //began..moved...etc

  int x;
  int y;
  int prev_x;
  int prev_y;
  
  int tick_count;  //리플레이 구현에는 고정프레임 방식이 필요하니까
  float timestamp;
  int uid;  // 터치마다붙는 고유값

};

class TouchEventQueue {
public:
  TouchEventQueue(bool save_history = false);
  ~TouchEventQueue();

  const TouchEvent &Front() const;
  TouchEvent Get();
  void Push(const TouchEvent &evt);
  bool IsEmpty() const;

private:
  std::queue<TouchEvent> evt_queue_;
  bool save_history_;
  std::vector<TouchEvent> evt_history_;
};

} // namespace sora

#endif  // SORA_TOUCH_EVENT_H_