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
#ifndef BASE_KAREN_HEAP_H_
#define BASE_KAREN_HEAP_H_
#pragma once

#include <string>
#include "karen/karen_enum.h"

namespace karen {;
class Heap {
  typedef unsigned char byte;

 public:
  Heap();
  explicit Heap(const std::string &name);
  ~Heap();
  const std::string &GetName();
  int GetId() const;

  void* Malloc(size_t size);
  void Free(void *ptr);

  static Heap *GetSharedHeap();

  void AddAllocation(size_t size);
  void RemoveAllocation(size_t size);
  int GetAllocatedMemSize() const;

  template<typename T>
  static HeapAllocHeader *GetHeader(const T *ptr);

 private:
  int id_;
  std::string name_;
  int allocated_size_;
  static int next_id_;
};
}

namespace karen {;
template<typename T>
HeapAllocHeader *Heap::GetHeader(const T *ptr) {
  HeapAllocHeader *header = reinterpret_cast<HeapAllocHeader*>(
    (byte*)(ptr) - sizeof(HeapAllocHeader));
  return header;
}
}
#endif  // BASE_KAREN_HEAP_H_
