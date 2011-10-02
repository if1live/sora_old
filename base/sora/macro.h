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
#ifndef BASE_SORA_MACRO_H_
#define BASE_SORA_MACRO_H_

#include <cassert>
#include "sora/platform.h"

// custom assert
#ifndef ST_ASSERT
#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while (0)
#endif
/*
#undef SR_ASSERT
#if RELEASE
#define SR_ASSERT(STATMENT) do { (void)sizeof(STATMENT); } while(0)
#else
#if SR_IOS
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
#elif SR_WIN
//#if USE_GTEST
//ref gpg 1
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
//extern bool CustomAssertFunction(bool, char*, int, char*);
#define SR_ASSERT(STATMENT) \
if(CustomAssertFunction((int)(STATMENT), "assert occur", __LINE__, __FILE__))	{ _asm { int 3 } }	///debugger stop
//내장 assert를 쓰지 못한 이유는 google test가 assert의 함수를 바꿔치기하는데 이것때문에 assert에 걸려도
//프로그램이 멈추지않고 그냥 죽어버린다. 그래서 강제로 죽이기 위해서 도입
#define SR_ASSERT(STATMENT) if(!(STATMENT)) { _asm { int 3} }
//#else
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
//#endif
#else
#error "cannot use assert"
#endif
#endif
*/

// auto getter/setter macro
#ifndef SR_GETTER_SETTER
#define SR_GETTER_SETTER(TYPE, NAME)  \
  public: \
  const TYPE &##NAME() const { return NAME##_; }  \
  void set_##NAME(const TYPE &v) { NAME##_ = v; } \
private:  \
  TYPE NAME##_
#endif

#ifndef SR_GETTER
#define SR_GETTER(TYPE, NAME) \
public: \
  const TYPE &##NAME() const { return NAME##_; }  \
private:  \
  void set_##NAME(const TYPE &v) { NAME##_ = v; } \
  TYPE NAME##_
#endif

#ifndef SR_SETTER
#define SR_SETTER(TYPE, NAME) \
public: \
  void set_##NAME(const TYPE &v) { NAME##_ = v; } \
private:  \
  const TYPE &##NAME() const { return NAME##_; }  \
  TYPE NAME##_
#endif

#endif  // BASE_SORA_MACRO_H_
