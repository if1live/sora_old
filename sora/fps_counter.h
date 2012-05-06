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
#pragma once

namespace sora {;

class FpsCounter {
public:
  FpsCounter();
  ~FpsCounter();

  void EndFrame(float frame_delta_time);
  float GetFPS() const { return fps_; }
  int total_frame_count() const { return count_; }
  int elapsed_millisecond() const { return elapsed_millisecond_; }
  float elapsed_second() const { return elapsed_millisecond_ / 1000.0f; }

private:
  int count_;
  int elapsed_millisecond_;
  //그냥 count/elapsed로 돌릴경우, 순간적인 변화량에 제대로 대응할수 없다
  //그래서 프레임 측정은 일정단위(100프레임정도?)마다 1번씩 수행해서 값을 저장하도록 햇다
  float fps_;
  int prev_calculated_millisecond_;
};

} //namespace sora