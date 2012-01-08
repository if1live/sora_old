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

namespace sora {;
void *BasicMalloc(size_t size) {
  void *ptr = ::malloc(size);
  // printf("malloc : %x\n", ptr);
  return ptr;
}
void BasicFree(void *ptr) {
  // printf("free : %x\n", ptr);
  return ::free(ptr);
}
void *BasicCalloc( size_t num, size_t size ) {
  return ::calloc(num, size);
}
void *BasicRealloc( void *memblock, size_t size ) {
  return ::realloc(memblock, size);
}


const i16 MagicNumber = 1234;
const i16 NoTag = 0;

struct AllocHeader {
  AllocHeader()
    : prev(NULL),
    next(NULL),
    magic(MagicNumber),
    tag(0),
    size(0) {
    prev = this;
    next = this;
  }

  AllocHeader *prev;
  AllocHeader *next;
  i16 magic;
  i16 tag;
  i32 size;
};
AllocHeader mem_chain;
i32 alloc_bytes = 0;  // 할당받은 총 용량
i32 alloc_count = 0;  // 할당받은 조각의 갯수

void *Malloc(size_t size) {
  return TagMalloc(size, NoTag);
};

void Free(void *ptr) {
  void *alloc_start = (void*)((long)ptr - sizeof(AllocHeader));
  AllocHeader *header = (AllocHeader*)alloc_start;

  SR_ASSERT(header->magic == MagicNumber && "Free::BadMagic!");
  
  header->prev->next = header->next;
  header->next->prev = header->prev;

  alloc_bytes -= header->size;
  alloc_count --;

  MM_FREE(alloc_start);
  return;
}
void *TagMalloc(size_t size, i32 tag) {
  size_t request_size = size + sizeof(AllocHeader);
  void *ptr = MM_MALLOC(request_size);
  
  AllocHeader *header = (AllocHeader*)ptr;
  header->magic = MagicNumber;
  header->tag = tag;
  header->size = size;

  AllocHeader &root = mem_chain;
  header->prev = root.prev;
  header->next = &root;
  root.prev->next = header;
  root.prev = header;

  alloc_bytes += size;
  alloc_count++;

  void *result = (void*)((long)ptr + sizeof(AllocHeader));
  return result;
}
void TagFree(i32 tag) {
  if (tag == NoTag) {
    return;
  }

  AllocHeader *header = mem_chain.next;
  AllocHeader *next = NULL;
  for ( ; header != &mem_chain ; header = next) {
    next = header->next;
    if (header->tag == tag) {
      void *alloc_ptr = (void*)((long)header + sizeof(AllocHeader));
      Free(alloc_ptr);
    }
  }
}
void AllocStat(AllocState *data) {
  data->bytes = alloc_bytes;
  data->count = alloc_count;
}
}
