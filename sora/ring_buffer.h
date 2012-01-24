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
#ifndef SORA_RING_BUFFER_H_
#define SORA_RING_BUFFER_H_

namespace sora {;
template<typename T, unsigned int N>
class RingBuffer {
public:
  enum {
    BufferSize = N,
  };
public:
  RingBuffer() : index_(-1) { }
  ~RingBuffer()	{ }

  bool IsEmpty() {
    if(index_ == -1) {
      return true;
    } else {
      return false;
    }
  }
  void Push(const T &value) {
    int target = (index_ + 1) % N;
    data_[target] = value;
    index_ = target;
  }
  const T& Top() const { return data_[index_]; }
  T &Top() { return data_[index_]; }

  const T& Get(int i) const {
    int target = (index_ - i + N) % N;
    return data_[target];
  }
  T& Get(int i) {
    int target = (index_ - i + N) % N;
    return data_[target];
  }

private:
  T data_[N];
  int index_;
};
}

#endif  // SORA_RING_BUFFER_H_