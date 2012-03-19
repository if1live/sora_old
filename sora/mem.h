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
#ifndef SORA_MEM_H_
#define SORA_MEM_H_

#include <cstddef>

namespace sora {;

// memory alloc stat
struct AllocState {
  AllocState() : bytes(0), count(0) {}
  int bytes;
  int count;
};

// low level alloc, support only simple log
// replace basic alloc
namespace BasicAllocator {;
SR_C_DLL void *Malloc(size_t size);
SR_C_DLL void Free(void *ptr);
SR_C_DLL void *Calloc( size_t num, size_t size );
SR_C_DLL void *Realloc( void *memblock, size_t size );
SR_C_DLL AllocState &alloc_state();
} //namespace BasicAlloc

// add header alloc. tag계열에서 삭제
struct TagAllocatorImpl;
class SR_DLL TagAllocator {
public:
  enum {
    kMagicNumber = 1234,
    kMaxTag = 16,
  };
  static void *Malloc(size_t size, int tag);
  static void Free(void *ptr);
  static void Flush(int tag);  //동일한 tag를 달고잇는거 한방에 삭제
  static AllocState &tag_alloc_state(int tag);
  static void TotalAllocStat(AllocState *data);

private:
  static TagAllocatorImpl &impl();
};

}

#define SR_MALLOC(X) sora::BasicAllocator::Malloc(X)
#define SR_FREE(X)  sora::BasicAllocator::Free(X)
#define SR_CALLOC(NUM, SIZE)  sora::BasicAllocator::Calloc(NUM, SIZE)
#define SR_REALLOC(MEMBLOCK, SIZE)  sora::BasicAllocator::Realloc(MEMBLOCK, SIZE)


#define SR_TAG_MALLOC(SIZE, TAG)  sora::TagAllocator::Malloc(SIZE, TAG)
#define SR_TAG_FREE(X)  sora::TagAllocator::Free(X)
#define SR_TAG_FLUSH(TAG)  sora::TagAllocator::Flush(TAG)


#endif  // SORA_MEM_H_
