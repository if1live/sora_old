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
#ifndef SORA_FREE_LIST_MANAGER_H_
#define SORA_FREE_LIST_MANAGER_H_

#include "assert_inc.h"
#include "free_list_block_pool.h"

namespace sora {;
template<typename T, int N>
class FreeListManager {
 public:
  void *operator new(size_t x);
  void operator delete(void *ptr);
 private:
  static FreeListBlockPool pool_;
};
}

namespace sora {;
template<typename T, int N>
void *FreeListManager<T, N>::operator new(size_t x) {
  void *ptr = pool_.Malloc();
  return ptr;
}

template<typename T, int N>
void FreeListManager<T, N>::operator delete(void *ptr) {
  pool_.Free(ptr);
}

template<typename T, int N>
FreeListBlockPool FreeListManager<T, N>::pool_(N, sizeof(T));
}

#endif  // SORA_FREE_LIST_MANAGER_H_
