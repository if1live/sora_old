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
#include "fps_counter.h"
#include "timer.h"

namespace sora {;
FpsCounter::FpsCounter()
: count_(0), 
elapsed_millisecond_(0),
fps_(0) {

}
FpsCounter::~FpsCounter() {

}

void FpsCounter::EndFrame(float frame_delta_time) {
  elapsed_millisecond_ += static_cast<int>(frame_delta_time * 1000);
  const int kInterval = 30;
  if(++count_ % kInterval == 0) {
    int interval_ms = elapsed_millisecond_ - prev_calculated_millisecond_;
    float interval_sec = interval_ms / 1000.0f;
    fps_ = kInterval / interval_sec;
    prev_calculated_millisecond_ = elapsed_millisecond_;
  }
}
} //namespace sora
