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
#ifndef SORA_TIMER_H_
#define SORA_TIMER_H_

#include "template_lib.h"

//c style
SR_C_DLL int Timer_GetMilliSecond();
SR_C_DLL float Timer_GetSecond();
SR_C_DLL void Timer_Init();
SR_C_DLL void Timer_Tick();

namespace sora {;
class Timer : public Singleton<Timer> {
public:
  Timer();
  ~Timer();

  static int GetMilliSecond();
  static float GetSecond();

  static void Tick();   //system call

private:
  static int GetSysMilliSecond(); //system call로 얻은 시간을 반환

  int start_ms_;
  int curr_ms_;
};
}

#endif  // SORA_TIMER_H_