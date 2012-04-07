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
#ifndef SORA_EVENT_QUEUE_H_
#define SORA_EVENT_QUEUE_H_

namespace sora {;
template<typename T>
class EventQueue {
public:
  EventQueue(bool save_history = false);
  ~EventQueue();

  const T &Front() const;
  T Get();
  void Push(const T &evt);
  bool IsEmpty() const;
  void Clear();

private:
  std::queue<T> evt_queue_;
  bool save_history_;
  std::vector<T> evt_history_;
};

//impl
template<typename T>
EventQueue<T>::EventQueue(bool save_history)
  : save_history_(save_history) {
}

template<typename T>
EventQueue<T>::~EventQueue() {
}

template<typename T>
const T &EventQueue<T>::Front() const {
  if(IsEmpty()) {
    //return fake event
    static T fake_evt;
    return fake_evt;
  } else {
    return evt_queue_.front();
  }
}

template<typename T>
T EventQueue<T>::Get() {
  T evt = Front();
  if(evt_queue_.empty() == false) {
    evt_queue_.pop();
  }
  return evt;
}

template<typename T>
void EventQueue<T>::Push(const T &evt) {
  evt_queue_.push(evt);
  if(save_history_) {
    evt_history_.push_back(evt);
  }
}

template<typename T>
bool EventQueue<T>::IsEmpty() const {
  return evt_queue_.empty();
}

template<typename T>
void EventQueue<T>::Clear() {
  evt_queue_ = std::queue<T>();
  evt_history_.clear();
}

} // namespace sora


#endif  // SORA_EVENT_QUEUE_H_