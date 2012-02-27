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

namespace sora {;
// low level alloc, support only simple log
void *BasicMalloc(size_t size);
void BasicFree(void *ptr);
void *BasicCalloc( size_t num, size_t size );
void *BasicRealloc( void *memblock, size_t size );

// add header alloc
void *Malloc(size_t size);
void Free(void *ptr);
void *TagMalloc(size_t size, i32 tag);
void TagFree(i32 tag);

// memory alloc stat
struct AllocState {
  i32 bytes;
  i32 count;
};
void AllocStat(AllocState *data);
}

#define MM_MALLOC(X) sora::BasicMalloc(X)
#define MM_FREE(X)  sora::BasicFree(X)
#define MM_CALLOC(NUM, SIZE)  sora::BasicCalloc(NUM, SIZE)
#define MM_REALLOC(MEMBLOCK, SIZE)  sora::BasicRealloc(MEMBLOCK, SIZE)

#define SR_MALLOC(X) sora::Malloc(X)
#define SR_FREE(X)  sora::Free(X)
#define SR_TAG_MALLOC(SIZE, TAG)  sora::TagMalloc(SIZE, TAG)
#define SR_TAG_FREE(TAG)  sora::TagFree(TAG)

#endif  // SORA_MEM_H_
