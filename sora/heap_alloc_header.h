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
#ifndef SORA_HEAP_ALLOC_HEADER_H_
#define SORA_HEAP_ALLOC_HEADER_H_

namespace sora {;
class Heap;

enum {
  kAllocatedMemorySignature = 0x12345678,
};

struct HeapAllocHeader {
 public:
  explicit HeapAllocHeader()
    : signature(kAllocatedMemorySignature),
    size(0),
    heap(NULL) {
  }
  ~HeapAllocHeader() {}
  void Reset(int signature, int size, Heap *heap) {
    this->signature = signature;
    this->size = size;
    this->heap = heap;
  }

 public:
  int signature;
  int size;
  Heap *heap;
};
}
#endif  // SORA_HEAP_ALLOC_HEADER_H_
