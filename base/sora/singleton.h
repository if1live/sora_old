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
#ifndef BASE_SORA_SINGLETON_H_
#define BASE_SORA_SINGLETON_H_

#include "sora/macro.h"

namespace sora {;
template<typename T>
class Singleton {
 public:
  static T& GetInstance() {
    if (ctx_ == NULL) {
      ctx_ = new T;
    }
    return *ctx_;
  }

  static void DestroyInstance() {
    if (ctx_ != NULL) {
      delete(ctx_);
      ctx_ = NULL;
    }
  }
  static bool IsCreated() {
    if (ctx_ == NULL) {
      return false;
    } else {
      return true;
    }
  }

 protected:
  Singleton() {
    SR_ASSERT(ctx_ == NULL);
    long offset = (long)(T*)(1) - (long)(Singleton*)(T*)(1);
    ctx_ = (T*)((long)(this + offset));
  }
  ~Singleton() {
    SR_ASSERT(ctx_ != NULL);
    ctx_ = NULL;
  }
  Singleton(const Singleton &o);
  Singleton operator=(const Singleton &o);

  static T *ctx_;
};

template<typename T>
T* Singleton<T>::ctx_ = NULL;
}

#endif  // BASE_SORA_SINGLETON_H_
