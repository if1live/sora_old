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
#include "stack_allocator.h"

#include "mem.h"
#include "template_lib.h"

namespace sora {;

//할당한 순서의 역순으로 해제되어야 스택이 꺠지지 않는다
//디버그 모드에서는 이를 확실히 확인하기 위해서 추가 속성을 할당햇다
struct StackAllocHeader {
  StackAllocHeader() : alloc_size(0) {}
  int alloc_size;
};

StackAllocator::StackAllocator(int stack_size) 
: data_(NULL),
top_(NULL), 
stack_size_(stack_size) {
  data_ = static_cast<unsigned char*>(sora::global_malloc(stack_size));
  top_  = data_;
}
StackAllocator::~StackAllocator() {
  if(data_ != NULL) {
    sora::global_free(data_);
    data_ = NULL;
  }
}

void *StackAllocator::Malloc(size_t x) {
  int alloc_size = x + sizeof(StackAllocHeader);

  //할당 가능한 크기가 x보다 작아야 할당 가능
  int curr_allocated_size = top_ - data_;
  int avail_alloc_size = stack_size_ - curr_allocated_size;
  if(avail_alloc_size < alloc_size) {
    SR_ASSERT(!"cannot alloc, more big stack required");
    return NULL;
  }

  uchar *raw_data = top_;
  top_ += alloc_size;
  StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>(raw_data);
  header->alloc_size = x;
  uchar *ptr = (uchar*)raw_data + sizeof(StackAllocHeader);
  
  return ptr;
}

void StackAllocator::Free(void *ptr) {
  //data가 stack의 범위에 존재하는 메모리 좌표인가?
  SR_ASSERT((uchar*)data_ <= ptr);
  SR_ASSERT((uchar*)data_ + stack_size_ > ptr);

  StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>((uchar*)ptr - sizeof(StackAllocHeader));
  int alloc_size = header->alloc_size;
  //가장마지막에 할당한것을 해제한다면 top와 할당크기가 연관되어잇다.
  //그렇지 않다면 top가 올바르지 않을것이다
  int expected_alloc_size = (uchar*)top_ - (uchar*)ptr;
  if(expected_alloc_size == alloc_size) {
    top_ -= sizeof(StackAllocHeader) + alloc_size;
  } else {
    SR_ASSERT(!"stack break?");
  }

}

void StackAllocator::FreeToMarker(Marker marker) {
  uchar *marker_ptr = reinterpret_cast<uchar*>(marker);
  SR_ASSERT(top_ >= marker_ptr);
  top_ = marker_ptr;
}

StackAllocator::Marker StackAllocator::GetMarker() {
  return reinterpret_cast<Marker>(top_);
}
int StackAllocator::GetAllocHeaderSize() const {
  return sizeof(StackAllocHeader);
}

void StackAllocator::Clear() {
  top_ = data_;
}
} //namespace sora