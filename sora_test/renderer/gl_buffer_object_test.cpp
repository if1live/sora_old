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
#include "renderer/gl/gl_buffer_object.h"

#include "core/vertex.h"

TEST(GLBufferData, test) {
  using namespace sora::gl;
  using namespace sora;
  using namespace std;

  vector<Vertex2D> vert_list;
  vert_list.push_back(CreateVertex2D(0, 0, 0, 0));
  vert_list.push_back(CreateVertex2D(1, 0, 0, 1));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 0));

  GLVertexBufferObject::HandleType handle = 0;
  EXPECT_EQ(false, GLVertexBufferObject::Loaded(handle));

  GLVertexBufferObject::Init(&handle, sizeof(Vertex2D) * vert_list.size(), &vert_list[0], GL_STATIC_DRAW);
  EXPECT_EQ(true, GLVertexBufferObject::Loaded(handle));

  GLVertexBufferObject::Deinit(&handle);
  EXPECT_EQ(false, GLVertexBufferObject::Loaded(handle));
}