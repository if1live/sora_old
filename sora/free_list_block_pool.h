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
#ifndef SORA_FREE_LIST_BLOCK_POOL_H_
#define SORA_FREE_LIST_BLOCK_POOL_H_

namespace sora {;
/// @brief reference GPG 4. free list memory manager
/// this class use malloc/free to requset/destory memory
/// if you want to constructor not called, use this class
/// for example, class operator new/delete overloading
class SR_DLL FreeListBlockPool {
 private:
  typedef unsigned char byte;
 public:
  explicit FreeListBlockPool(int max_size, int block_size);
  ~FreeListBlockPool();
  void Reset(int max_size, int block_size);
  void* Malloc();
  void Free(void *ptr);
 private:
  byte *object_list_;
  byte **free_object_list_;
  int max_size_;
  int block_size_;
  int top_;
};
}
#endif  // SORA_FREE_LIST_BLOCK_POOL_H_
