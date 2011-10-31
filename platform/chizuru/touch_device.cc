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
#include "chizuru/touch_device.h"
#include <algorithm>

using std::vector;
using std::find;

namespace chizuru {;
TouchDevice::TouchDevice() {
}
TouchDevice::~TouchDevice() {
}
void TouchDevice::TouchBegan(const std::vector<TouchEvent> &touches) {
  ListenerListType::const_iterator it = listener_list_.begin();
  ListenerListType::const_iterator endit = listener_list_.end();
  for ( ; it != endit ; it++) {
    (*it)->TouchBegan(touches);
  }
}
void TouchDevice::TouchMoved(const std::vector<TouchEvent> &touches) {
  ListenerListType::const_iterator it = listener_list_.begin();
  ListenerListType::const_iterator endit = listener_list_.end();
  for ( ; it != endit ; it++) {
    (*it)->TouchMoved(touches);
  }
}
void TouchDevice::TouchEnded(const std::vector<TouchEvent> &touches) {
  ListenerListType::const_iterator it = listener_list_.begin();
  ListenerListType::const_iterator endit = listener_list_.end();
  for ( ; it != endit ; it++) {
    (*it)->TouchEnded(touches);
  }
}
void TouchDevice::TouchCancelled(const std::vector<TouchEvent> &touches) {
  ListenerListType::const_iterator it = listener_list_.begin();
  ListenerListType::const_iterator endit = listener_list_.end();
  for ( ; it != endit ; it++) {
    (*it)->TouchCancelled(touches);
  }
}

bool TouchDevice::AddListener(TouchListener *listener) {
  if (listener == NULL) {
    return false;
  }
  vector<TouchListener*>::iterator found = find(listener_list_.begin(),
    listener_list_.end(), listener);
  if (found != listener_list_.end()) {
    return false;
  } else {
    listener_list_.push_back(listener);
    return true;
  }
}
bool TouchDevice::RemoveListener(TouchListener *listener) {
  if (listener == NULL) {
    return false;
  }
  vector<TouchListener*>::iterator found = find(listener_list_.begin(),
    listener_list_.end(), listener);
  if (found != listener_list_.end()) {
    listener_list_.erase(found);
    return true;
  } else {
    return false;
  }
}
void TouchDevice::ClearListener() {
  listener_list_.clear();
}
}
