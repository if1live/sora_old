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
#include "free_list_block_pool.h"
#include "mem.h"

namespace sora {;
FreeListBlockPool::FreeListBlockPool(int max_size, int block_size)
  : object_list_(NULL),
  free_object_list_(NULL),
  max_size_(max_size),
  top_(0),
  block_size_(block_size) {
  Reset(max_size, block_size);
}

FreeListBlockPool::~FreeListBlockPool() {
  // release all obj at once
  // SR_ASSERT(max_size_ == freeListSize_);
  sora::global_free(free_object_list_);
  sora::global_free(object_list_);
  object_list_ = NULL;
  free_object_list_ = NULL;
}

void FreeListBlockPool::Reset(int max_size, int block_size) {
  if (object_list_ != NULL) {
    // all previous allocated block must be unused
    SR_ASSERT(max_size_ == top_+1);
    sora::global_free(free_object_list_);
    sora::global_free(object_list_);
  }
  // reallocate
  max_size_ = max_size;

  object_list_ = static_cast<byte*>(
    sora::global_malloc(sizeof(object_list_) * max_size * block_size));
  memset(object_list_, 0, sizeof(byte) * max_size * block_size);
  free_object_list_ = static_cast<byte**>(
    sora::global_malloc(sizeof(object_list_) * max_size));
  memset(free_object_list_, 0, sizeof(byte*) * max_size);

  for (int i = 0 ; i < max_size_ ; i++) {
    byte *ptr = (object_list_ + i*block_size);
    free_object_list_[i] = ptr;
  }
  top_ = max_size-1;
}

void *FreeListBlockPool::Malloc() {
  SR_ASSERT(top_ >= 0 && "no free block exist");
  byte *ptr = free_object_list_[top_];
  top_--;
  return reinterpret_cast<void*>(ptr);
}

void FreeListBlockPool::Free(void *ptr) {
#if DEBUG
  bool found = false;
  // ptr must be member of object_list
  for (int i = 0 ; i < max_size_ ; i++) {
    if ((object_list_ + i*block_size_) == ptr) {
      found = true;
    }
  }
  SR_ASSERT(found == true);
#endif
  top_++;
  free_object_list_[top_] = reinterpret_cast<byte*>(ptr);
}
}
