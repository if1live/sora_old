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
#include "sora_stdafx.h"
#include "heap.h"
#include "heap_alloc_header.h"

namespace sora {;
int Heap::next_id_ = 1;
Heap::Heap()
  : name_(""), allocated_size_(0), id_(next_id_++) {
}
Heap::Heap(const std::string &name)
  : name_(name), allocated_size_(0), id_(next_id_++) {
}
Heap::~Heap() {
}
const std::string &Heap::GetName() {
  return name_;
}
int Heap::GetId() const {
  return id_;
}
Heap *Heap::GetSharedHeap() {
  using std::string;
  static Heap h(string("default"));
  return &h;
}
void* Heap::Malloc(size_t size) {
#if SR_DEBUG
  int requestSize = size + sizeof(HeapAllocHeader);
  byte *mem = reinterpret_cast<byte*>(::malloc(requestSize));
  HeapAllocHeader *header = reinterpret_cast<HeapAllocHeader*>(mem);
  header->Reset(kAllocatedMemorySignature, size, this);
  this->AddAllocation(size);

  void *startBlock = mem + sizeof(HeapAllocHeader);
  return startBlock;
#else
  byte *mem = reinterpret_cast<byte*>(::malloc(size));
  return mem;
#endif
}
void Heap::Free(void *ptr) {
#if SR_DEBUG
  HeapAllocHeader *header = reinterpret_cast<HeapAllocHeader*>(
    reinterpret_cast<byte*>(ptr) - sizeof(HeapAllocHeader));
  header->heap->RemoveAllocation(header->size);
  SR_ASSERT(header->signature == kAllocatedMemorySignature);
  ::free(header);
#else
  ::free(ptr);
#endif
}

void Heap::AddAllocation(size_t size) {
  allocated_size_ += size;
}
void Heap::RemoveAllocation(size_t size) {
  allocated_size_ -= size;
}
int Heap::GetAllocatedMemSize() const {
  return allocated_size_;
}
}
