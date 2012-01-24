﻿/*  Copyright (C) 2011 by if1live */
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
#ifndef SORA_ACCELEROMETER_H_
#define SORA_ACCELEROMETER_H_

#include "sora/template_library.h"
#include "sora/ring_buffer.h"

namespace sora {;
struct AccelData {
  union {
    struct {
      float x;
      float y;
      float z;
    };
    float value[3];
  };
};

class Accelerometer : public Singleton<Accelerometer> {
public:
  ~Accelerometer();
  Accelerometer();
  void Add(float x, float y, float z);
  const AccelData &GetLast() const;

private:
  RingBuffer<AccelData, 10> buffer_;
};

}

#endif  // SORA_ACCELEROMETER_H_