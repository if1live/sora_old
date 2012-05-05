﻿/*  Copyright (C) 2011 by if1live */
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
#include "stack_allocator.h"

using namespace sora;

TEST(StackAllocator, basic_malloc_free) {
  StackAllocator allocator;
  EXPECT_EQ(allocator.stack_size(), allocator.remain_size());
  const int header_size = allocator.GetAllocHeaderSize();

  void *ptr1 = allocator.Malloc(10);
  EXPECT_EQ(allocator.stack_size() - (10 + header_size), allocator.remain_size());
  
  void *ptr2 = allocator.Malloc(20);
  EXPECT_EQ(allocator.stack_size() - (20 + 10 + header_size * 2), allocator.remain_size());

  EXPECT_NE(20, (uchar*)ptr2 - (uchar*)ptr1);

  allocator.Free(ptr2);
  EXPECT_EQ(allocator.stack_size() - (10 + header_size), allocator.remain_size());

  allocator.Free(ptr1);
  EXPECT_EQ(allocator.stack_size(), allocator.remain_size());
}

/*
TEST(StackAllocator, reverse_free) {
  //ptr1, ptr2로 할당받고 순서를 뒤집어서 해제하는 경우, 스택이 깨지니 적절히 죽어야한다
  StackAllocator allocator;
  void *ptr1 = allocator.Malloc(10);
  void *ptr2 = allocator.Malloc(20);
  allocator.Free(ptr1);
  allocator.Free(ptr2);
}
*/

TEST(StackAllocator, marker) {
  StackAllocator allocator;
  void *ptr1 = allocator.Malloc(10);
  void *ptr2 = allocator.Malloc(20);
  StackAllocator::Marker makrer_a = allocator.GetMarker();
  int remain_size_1 = allocator.remain_size();
  void *ptr3 = allocator.Malloc(30);
  void *ptr4 = allocator.Malloc(400);
  allocator.FreeToMarker(makrer_a);
  EXPECT_EQ(remain_size_1, allocator.remain_size());
}

TEST(StackAllocator, Clear) {
  StackAllocator allocator;
  void *ptr1 = allocator.Malloc(10);
  allocator.Clear();
  void *ptr2 = allocator.Malloc(40);
  EXPECT_EQ(ptr1, ptr2);
}


TEST(DoubleStackAllocator, malloc_free) {
  DoubleStackAllocator allocator;
  const int header_size = allocator.GetAllocHeaderSize();
  const int stack_size = allocator.stack_size();
  EXPECT_EQ(stack_size, allocator.remain_size());

  void *ptr1 = allocator.LowerMalloc(10);
  EXPECT_EQ(stack_size - (10 + header_size), allocator.remain_size());

  void *ptr2 = allocator.LowerMalloc(20);
  EXPECT_EQ(stack_size - (10 + 20 + header_size * 2), allocator.remain_size());

  void *ptr3 = allocator.UpperMalloc(30);
  EXPECT_EQ(stack_size - (10 + 20 + 30 + header_size * 3), allocator.remain_size());

  void *ptr4 = allocator.UpperMalloc(20);
  EXPECT_EQ(stack_size - (10 + 20 + 30 + 20 + header_size * 4), allocator.remain_size());

  //free start
  allocator.LowerFree(ptr2);
  EXPECT_EQ(stack_size - (10 + 30 + 20 + header_size * 3), allocator.remain_size());

  allocator.UpperFree(ptr4);
  EXPECT_EQ(stack_size - (10 + 30 + header_size * 2), allocator.remain_size());

  allocator.UpperFree(ptr3);
  EXPECT_EQ(stack_size - (10 + header_size), allocator.remain_size());

  allocator.LowerFree(ptr1);
  EXPECT_EQ(stack_size, allocator.remain_size());
}

TEST(DoubleStackAllocator, Clear) {
  DoubleStackAllocator allocator;
  const int header_size = allocator.GetAllocHeaderSize();
  const int stack_size = allocator.stack_size();
  EXPECT_EQ(stack_size, allocator.remain_size());

  void *ptr1 = allocator.LowerMalloc(10);
  EXPECT_EQ(stack_size - (10 + header_size), allocator.remain_size());

  void *ptr2 = allocator.LowerMalloc(20);
  EXPECT_EQ(stack_size - (10 + 20 + header_size * 2), allocator.remain_size());

  void *ptr3 = allocator.UpperMalloc(30);
  EXPECT_EQ(stack_size - (10 + 20 + 30 + header_size * 3), allocator.remain_size());

  void *ptr4 = allocator.UpperMalloc(20);
  EXPECT_EQ(stack_size - (10 + 20 + 30 + 20 + header_size * 4), allocator.remain_size());

  //free start
  allocator.LowerClear();
  EXPECT_EQ(stack_size - (30 + 20 + header_size * 2), allocator.remain_size());

  allocator.UpperClear();
  EXPECT_EQ(stack_size, allocator.remain_size());
}

TEST(DoubleStackAllocator, Marker) {
  DoubleStackAllocator allocator;
  const int header_size = allocator.GetAllocHeaderSize();
  const int stack_size = allocator.stack_size();
  EXPECT_EQ(stack_size, allocator.remain_size());

  void *ptr1 = allocator.LowerMalloc(10);
  void *ptr2 = allocator.LowerMalloc(20);
  DoubleStackAllocator::Marker lower_marker = allocator.GetLowerMarker();
  void *ptr_lower_a = allocator.LowerMalloc(5);

  void *ptr3 = allocator.UpperMalloc(30);
  void *ptr4 = allocator.UpperMalloc(20);
  DoubleStackAllocator::Marker upper_marker = allocator.GetUpperMarker();
  void *ptr_upper_a = allocator.UpperMalloc(5);

  allocator.FreeToLowerMarker(lower_marker);
  void *ptr_lower_b = allocator.LowerMalloc(9);
  EXPECT_EQ(ptr_lower_a, ptr_lower_b);

  allocator.FreeToUpperMarker(upper_marker);
  void *ptr_upper_b = allocator.UpperMalloc(5);
  EXPECT_EQ(ptr_upper_a, ptr_upper_b);
}