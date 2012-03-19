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
#include "sora/mem.h"
#include <cstdlib>

namespace sora {;

namespace BasicAllocator {;
struct BasicAllocHeader {
  //경계정보 같은거 없음. 그냥 size만 추가해서 메모리 할당을 추적할수 잇도록함
  BasicAllocHeader() : size(0) { }
  BasicAllocHeader(int size) : size(size) { }
  int size;
};

//for basic allocator
AllocState basic_alloc_state;

AllocState &alloc_state() {
  return basic_alloc_state;
}

void *Malloc(size_t size) {
  size_t request_size = size + sizeof(BasicAllocHeader);
  void *ptr = ::malloc(request_size);
  BasicAllocHeader *header = (BasicAllocHeader*)ptr;
  header->size = size;
  void *result = (void*)((long)ptr + sizeof(BasicAllocHeader));
  
  basic_alloc_state.bytes += size;
  basic_alloc_state.count++;

  return result;
}
void Free(void *ptr) {
  void *alloc_start = (void*)((long)ptr - sizeof(BasicAllocHeader));
  BasicAllocHeader *header = (BasicAllocHeader*)alloc_start;
  
  int size = header->size;
  basic_alloc_state.bytes -= size;
  basic_alloc_state.count--;

  ::free(header);
}
void *Calloc( size_t num, size_t size ) {
  int request_size = num * size;
  void *ptr = SR_MALLOC(request_size);
  memset(ptr, 0, request_size);
  return ptr;
}
void *Realloc( void *memblock, size_t size ) {
  void *alloc_start = (void*)((long)memblock - sizeof(BasicAllocHeader));
  BasicAllocHeader *prev_header = (BasicAllocHeader*)alloc_start;

  int prev_size = prev_header->size;

  void *new_block = ::realloc(alloc_start, size);
  BasicAllocHeader *new_header = (BasicAllocHeader*)new_block;
  new_header->size = size;
  basic_alloc_state.bytes += (size - prev_size);

  void *result = (void*)((long)new_block + sizeof(BasicAllocHeader));
  return result;
}
}   // namespace BasicAllocator

//for tag allocator
struct TagAllocHeader {
  TagAllocHeader()
    : prev(NULL),
    next(NULL),
    magic(TagAllocator::kMagicNumber),
    tag(0),
    size(0) {
    prev = this;
    next = this;
  }

  TagAllocHeader *prev;
  TagAllocHeader *next;
  ushort magic;
  ushort tag;
  int size;
};

struct TagAllocatorImpl {
  AllocState tag_alloc_state_table[TagAllocator::kMaxTag];  //tag별로 할당상태 별로 관리
  TagAllocHeader mem_chain[TagAllocator::kMaxTag];  //tag는 총 16개
};

TagAllocatorImpl &TagAllocator::impl() {
  static TagAllocatorImpl ctx;
  return ctx;
}

AllocState &TagAllocator::tag_alloc_state(int tag) {
  SR_ASSERT(tag >= 0);
  SR_ASSERT(tag < kMaxTag);
  TagAllocatorImpl &ctx = impl();
  return ctx.tag_alloc_state_table[tag];
}

void TagAllocator::Free(void *ptr) {
  TagAllocatorImpl &ctx = impl();
  void *alloc_start = (void*)((long)ptr - sizeof(TagAllocHeader));
  TagAllocHeader *header = (TagAllocHeader*)alloc_start;

  SR_ASSERT(header->magic == kMagicNumber && "Free::BadMagic!");
  
  header->prev->next = header->next;
  header->next->prev = header->prev;

  ushort tag = header->tag;
  ctx.tag_alloc_state_table[tag].bytes -= header->size;
  ctx.tag_alloc_state_table[tag].count--;

  ::free(alloc_start);
  return;
}

void *TagAllocator::Malloc(size_t size, int tag) {
  SR_ASSERT(tag >= 0);
  SR_ASSERT(tag < kMaxTag);
  TagAllocatorImpl &ctx = impl();

  size_t request_size = size + sizeof(TagAllocHeader);
  void *ptr = ::malloc(request_size);
  
  TagAllocHeader *header = (TagAllocHeader*)ptr;
  header->magic = kMagicNumber;
  header->tag = tag;
  header->size = size;

  TagAllocHeader &root = ctx.mem_chain[tag];
  header->prev = root.prev;
  header->next = &root;
  root.prev->next = header;
  root.prev = header;

  ctx.tag_alloc_state_table[tag].bytes += header->size;
  ctx.tag_alloc_state_table[tag].count++;

  void *result = (void*)((long)ptr + sizeof(TagAllocHeader));
  return result;
}

void TagAllocator::Flush(int tag) {
  SR_ASSERT(tag >= 0);
  SR_ASSERT(tag < kMaxTag);
  TagAllocatorImpl &ctx = impl();

  TagAllocHeader *header = ctx.mem_chain[tag].next;
  TagAllocHeader *next = NULL;
  for ( ; header != &ctx.mem_chain[tag] ; header = next) {
    next = header->next;
    if (header->tag == tag) {
      void *alloc_ptr = (void*)((long)header + sizeof(TagAllocHeader));
      Free(alloc_ptr);
    }
  }
}
void TagAllocator::TotalAllocStat(AllocState *data) {
  memset(data, 0, sizeof(AllocState));
  TagAllocatorImpl &ctx = impl();
  for(int i = 0 ; i < kMaxTag ; i++) {
    data->bytes += ctx.tag_alloc_state_table[i].bytes;
    data->count += ctx.tag_alloc_state_table[i].count;
  }
}

} //namespace sora
