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
#include "sora/common/ring_buffer.h"

TEST(RingBuffer, test) {
  using namespace sora;

	RingBuffer<int, 3> buffer;
	EXPECT_EQ(true, buffer.IsEmpty());

	buffer.Push(1);
	EXPECT_EQ(false, buffer.IsEmpty());
	EXPECT_EQ(1, buffer.Get(0));

	buffer.Push(2);
	EXPECT_EQ(false, buffer.IsEmpty());
	EXPECT_EQ(2, buffer.Get(0));
	EXPECT_EQ(1, buffer.Get(1));

	buffer.Push(3);
	EXPECT_EQ(false, buffer.IsEmpty());
	EXPECT_EQ(3, buffer.Get(0));
	EXPECT_EQ(2, buffer.Get(1));
	EXPECT_EQ(1, buffer.Get(2));

	buffer.Push(4);
	EXPECT_EQ(false, buffer.IsEmpty());
	EXPECT_EQ(4, buffer.Get(0));
	EXPECT_EQ(3, buffer.Get(1));
	EXPECT_EQ(2, buffer.Get(2));
}