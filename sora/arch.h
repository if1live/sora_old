// Copyright (C) 2011 by if1live
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
#ifndef SORA_ARCH_H_
#define SORA_ARCH_H_

// platform detect code
#undef SR_IOS
#undef SR_WIN
#undef SR_ANDROID
#define SR_IOS 0
#define SR_WIN 0
#define SR_ANDROID 0

// platform check macro
#if __APPLE__
// iphone check
// 이것은 가장 밑바닥에 인클루드될 헤더이므로
// 다른 헤더를 include하지 않는다.
// #import <Availability.h>
// #if __IPHONE_2_0
#undef SR_IOS
#define SR_IOS 1
// #endif
#endif

// android check
#ifdef __ANDROID__
#undef SR_ANDROID
#define SR_ANDROID 1
#endif

#if (defined _WIN32) || (defined _W64)
#undef SR_WIN
#define SR_WIN 1
#endif

#if SR_IOS == 1 || SR_WIN == 1
#define SR_USE_PCH 1
#else
#define SR_USE_PCH 0
#endif

namespace sora {;
//custom type
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef bool boolean;
typedef float f32;
typedef double f64;

//file position
enum {
  kPositionStart,
  kPositionCurrent,
  kPositionEnd
};
int PositionToWhere(int pos);
}

#endif  // SORA_ARCH_H_
