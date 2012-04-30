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
#ifndef SORA_FREE_LIST_OBJECT_POOL_H_
#define SORA_FREE_LIST_OBJECT_POOL_H_

#include "assert_inc.h"

namespace sora {;
/// @brief reference GPG 4. free list memory manager
/// this class use new/delete to requset/destory memory
/// if constructor call in pool initializion is not important, use this class
/// @param N max pool size
template<typename T>
class FreeListObjectPool {
 public:
  explicit FreeListObjectPool(int max_size);
  ~FreeListObjectPool();
  void Reset(int max_size);
  T *Malloc();
  void Free(T *ptr);
 private:
  T *object_list_;
  T **free_object_list_;
  int max_size_;
  int top_;
};
}

namespace sora {;
template<typename T>
FreeListObjectPool<T>::FreeListObjectPool(int max_size)
  : object_list_(NULL),
  free_object_list_(NULL),
  max_size_(0),
  top_(0) {
  Reset(max_size);
}
template<typename T>
FreeListObjectPool<T>::~FreeListObjectPool() {
  // release all obj at once
  // SR_ASSERT(max_size_ == freeListSize_);
  delete[](object_list_);
  delete[](free_object_list_);
  object_list_ = NULL;
  free_object_list_ = NULL;
}

template<typename T>
void FreeListObjectPool<T>::Reset(int max_size) {
  if (object_list_ != NULL) {
    // all previous allocated block must be unused
    SR_ASSERT(max_size_ == top_+1);
    delete[](object_list_);
    delete[](free_object_list_);
  }
  // reallocate
  max_size_ = max_size;

  object_list_ = new T[max_size];
  free_object_list_ = new T*[max_size];

  for (int i = 0 ; i < max_size_ ; i++) {
    free_object_list_[i] = &(object_list_[i]);
  }
  top_ = max_size_-1;
}
template<typename T>
T *FreeListObjectPool<T>::Malloc() {
  SR_ASSERT(top_ >= 0 && "no free block exist");
  T *ptr = free_object_list_[top_];
  top_--;
  return ptr;
}
template<typename T>
void FreeListObjectPool<T>::Free(T *ptr) {
#if DEBUG
  bool found = false;
  // ptr must be member of object_list
  for (int i = 0 ; i < max_size_ ; i++) {
    if (&object_list_[i] == ptr) {
      found = true;
    }
  }
  SR_ASSERT(found == true);
#endif
  top_++;
  free_object_list_[top_] = ptr;
}
}

#endif  // SORA_FREE_LIST_OBJECT_POOL_H_
