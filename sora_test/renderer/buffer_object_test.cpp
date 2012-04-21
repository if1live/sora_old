/*  Copyright (C) 2011-2012 by if1live */
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
#include "renderer/buffer_object.h"

using namespace std;
using namespace sora;

TEST(BufferObject, vbo) {
  typedef VertexListSelector<Vertex2D>::Result VertType;
  VertType vert_list;
  vert_list.push_back(Vertex2D(0, 0, 0, 0));
  vert_list.push_back(Vertex2D(1, 0, 0, 1));
  vert_list.push_back(Vertex2D(1, 1, 1, 0));

  VBOSelector<VertType::value_type>::Result vbo;
  EXPECT_EQ(false, vbo.Loaded());
  vbo.Init(vert_list, kBufferUsageStatic);
  EXPECT_EQ(true, vbo.Loaded());
  vbo.Deinit();
  EXPECT_EQ(false, vbo.Loaded());
}

TEST(BufferObject, ibo) {
  IndexList index_list;
  index_list.push_back(1);
  index_list.push_back(4);
  index_list.push_back(5);

  IndexBufferObject ibo;
  EXPECT_EQ(false, ibo.Loaded());
  ibo.Init(index_list, kBufferUsageStatic);
  EXPECT_EQ(true, ibo.Loaded());
  ibo.Deinit();
  EXPECT_EQ(false, ibo.Loaded());
}