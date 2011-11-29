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
#include "sora_test_stdafx.h"
#include "sora/mem.h"

TEST(Memory, Malloc_Free) {
  using sora::AllocStat;
  sora::AllocState alloc_state;
  
  AllocStat(&alloc_state);
  EXPECT_EQ(0, alloc_state.bytes);
  EXPECT_EQ(0, alloc_state.count);

  void *ptr1 = SR_MALLOC(4);
  AllocStat(&alloc_state);
  EXPECT_EQ(4, alloc_state.bytes);
  EXPECT_EQ(1, alloc_state.count);

  void *ptr2 = SR_MALLOC(8);
  AllocStat(&alloc_state);
  EXPECT_EQ(4+8, alloc_state.bytes);
  EXPECT_EQ(2, alloc_state.count);

  void *ptr3 = SR_MALLOC(16);
  AllocStat(&alloc_state);
  EXPECT_EQ(4+8+16, alloc_state.bytes);
  EXPECT_EQ(3, alloc_state.count);

  void *ptr4 = SR_MALLOC(32);
  AllocStat(&alloc_state);
  EXPECT_EQ(4+8+16+32, alloc_state.bytes);
  EXPECT_EQ(4, alloc_state.count);

  SR_FREE(ptr1);  // 4byte
  AllocStat(&alloc_state);
  EXPECT_EQ(8+16+32, alloc_state.bytes);
  EXPECT_EQ(3, alloc_state.count);

  SR_FREE(ptr4);  // 32byte
  AllocStat(&alloc_state);
  EXPECT_EQ(8+16, alloc_state.bytes);
  EXPECT_EQ(2, alloc_state.count);

  SR_FREE(ptr2);  // 8byte
  AllocStat(&alloc_state);
  EXPECT_EQ(16, alloc_state.bytes);
  EXPECT_EQ(1, alloc_state.count);

  SR_FREE(ptr3);  // 16byte
  AllocStat(&alloc_state);
  EXPECT_EQ(0, alloc_state.bytes);
  EXPECT_EQ(0, alloc_state.count);
}

TEST(Memory, Malloc_Free_1) {
  using sora::AllocStat;
  sora::AllocState alloc_state;
  
  AllocStat(&alloc_state);
  EXPECT_EQ(0, alloc_state.bytes);
  EXPECT_EQ(0, alloc_state.count);

  void *ptr1 = SR_MALLOC(4);
  AllocStat(&alloc_state);
  EXPECT_EQ(4, alloc_state.bytes);
  EXPECT_EQ(1, alloc_state.count);

  void *ptr2 = SR_MALLOC(8);
  AllocStat(&alloc_state);
  EXPECT_EQ(4+8, alloc_state.bytes);
  EXPECT_EQ(2, alloc_state.count);

  ///////////
  SR_FREE(ptr1);  // 4byte
  AllocStat(&alloc_state);
  EXPECT_EQ(8, alloc_state.bytes);
  EXPECT_EQ(1, alloc_state.count);

  void *ptr3 = SR_MALLOC(16);
  AllocStat(&alloc_state);
  EXPECT_EQ(8+16, alloc_state.bytes);
  EXPECT_EQ(2, alloc_state.count);

  void *ptr4 = SR_MALLOC(32);
  AllocStat(&alloc_state);
  EXPECT_EQ(8+16+32, alloc_state.bytes);
  EXPECT_EQ(3, alloc_state.count);

  SR_FREE(ptr3);  // 16byte
  AllocStat(&alloc_state);
  EXPECT_EQ(8+32, alloc_state.bytes);
  EXPECT_EQ(2, alloc_state.count);

  SR_FREE(ptr2);  // 8byte
  AllocStat(&alloc_state);
  EXPECT_EQ(32, alloc_state.bytes);
  EXPECT_EQ(1, alloc_state.count);

  SR_FREE(ptr4);  // 32byte
  AllocStat(&alloc_state);
  EXPECT_EQ(0, alloc_state.bytes);
  EXPECT_EQ(0, alloc_state.count);
}

TEST(Memory, TagMalloc_TagFree) {
  using sora::AllocStat;
  sora::AllocState alloc_state;

  void *ptr1 = SR_TAG_MALLOC(4, 1);
  void *ptr2 = SR_TAG_MALLOC(8, 2);
  void *ptr3 = SR_TAG_MALLOC(16, 1);
  void *ptr4 = SR_TAG_MALLOC(32, 2);
  
  AllocStat(&alloc_state);
  EXPECT_EQ(4+8+16+32, alloc_state.bytes);
  EXPECT_EQ(4, alloc_state.count);
  
  SR_TAG_FREE(2);
  AllocStat(&alloc_state);
  EXPECT_EQ(4+16, alloc_state.bytes);
  EXPECT_EQ(2, alloc_state.count);

  SR_TAG_FREE(1);
  AllocStat(&alloc_state);
  EXPECT_EQ(0, alloc_state.bytes);
  EXPECT_EQ(0, alloc_state.count);
}