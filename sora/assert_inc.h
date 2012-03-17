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
#ifndef SORA_ASSERT_INC_H_
#define SORA_ASSERT_INC_H_

#include "arch.h"
#include <cassert>
#include <cstdlib>

#if SR_WIN
//#define SR_ASSERT(EXPR) do { assert(EXPR); } while(0);
#define SR_ASSERT(EXPR) if(!(EXPR)) { __asm{ int 3 }; }

#elif SR_IOS
#define SR_ASSERT(EXPR) do { assert(EXPR); } while(0);

#elif SR_ANDROID
#include "logger.h"
#define SR_ASSERT(EXPR) do { \
  if (!(EXPR)) {  \
    LOGE("%s : %d : %s", __FILE__, __LINE__, #EXPR); \
    assert(EXPR); \
    exit(-1); \
  } } while(0);

#else
#error "not support"
#endif

//static_assert
#if SR_WIN || SR_IOS
#define SR_STATIC_ASSERT(A, B) static_assert(A, B)
#define SR_STATIC_ASSERT_NOMSG(A) static_assert(A, "no msg")
#else
#define SR_STATIC_ASSERT(A, B)
#define SR_STATIC_ASSERT_NOMSG(A)
#endif

#endif  // SORA_ASSERT_INC_H_
