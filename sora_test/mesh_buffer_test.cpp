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
#include "sora/vertex.h"
#include "sora/buffer_object.h"
#include "sora/mesh_buffer.h"

using namespace sora;
using namespace std;

TEST(VertexBufferInterface, std_vector_version) {
  vector<Vertex2D> vert_list;
  vert_list.push_back(CreateVertex2D(0, 0, 0, 0));
  vert_list.push_back(CreateVertex2D(0, 0, 0, 1));
  vert_list.push_back(CreateVertex2D(0, 0, 2, 0));

  unique_ptr<VertexBufferInterface> buffer(new VertexArrayT<Vertex2D>());
  buffer->Init(vert_list);
  EXPECT_EQ(vert_list.size(), buffer->size());
  EXPECT_EQ(vert_list.empty(), buffer->empty());
  EXPECT_EQ(false, buffer->IsBuffer());
  EXPECT_EQ(true, NULL != buffer->data());
  EXPECT_EQ(sizeof(Vertex2D), buffer->ElemSize());
  EXPECT_EQ(0, buffer->handle());
  EXPECT_EQ(kVertex2D, buffer->vertex_code());
  buffer->Deinit();
}
TEST(VertexBufferInterface, vbo_version) {
  vector<Vertex2D> vert_list;
  vert_list.push_back(CreateVertex2D(0, 0, 0, 0));
  vert_list.push_back(CreateVertex2D(0, 0, 0, 1));
  vert_list.push_back(CreateVertex2D(0, 0, 2, 0));

  unique_ptr<VertexBufferInterface> buffer(new Vertex2DBufferObject());
  buffer->Init(vert_list);
  EXPECT_EQ(vert_list.size(), buffer->size());
  EXPECT_EQ(vert_list.empty(), buffer->empty());
  EXPECT_EQ(true, buffer->IsBuffer());
  EXPECT_EQ(nullptr, buffer->data());
  EXPECT_EQ(sizeof(Vertex2D), buffer->ElemSize());
  EXPECT_EQ(true, 0 < buffer->handle());
  EXPECT_EQ(kVertex2D, buffer->vertex_code());
  buffer->Deinit();
}

TEST(IndexBufferInterface, std_vector_version) {
  IndexList index_list(1, 10);
  unique_ptr<IndexBufferInterface> buffer(new IndexArray());
  buffer->Init(index_list);
  EXPECT_EQ(index_list.size(), buffer->size());
  EXPECT_EQ(index_list.empty(), buffer->empty());
  EXPECT_EQ(false, buffer->IsBuffer());
  EXPECT_EQ(true, NULL != buffer->data());
  EXPECT_EQ(0, buffer->handle());
  buffer->Deinit();
}

TEST(IndexBufferInterface, ibo_version) {
  IndexList index_list(1, 10);
  unique_ptr<IndexBufferInterface> buffer(new IndexBufferObject());
  buffer->Init(index_list);
  EXPECT_EQ(index_list.size(), buffer->size());
  EXPECT_EQ(index_list.empty(), buffer->empty());
  EXPECT_EQ(true, buffer->IsBuffer());
  EXPECT_EQ(nullptr, buffer->data());
  EXPECT_EQ(true, 0 < buffer->handle());
  buffer->Deinit();
}

TEST(MeshBuffer, Register) {
  MeshBuffer mesh_buffer;
  int v_id_1 = mesh_buffer.Register(new Vertex2DBufferObject());
  int v_id_2 = mesh_buffer.Register(new VertexArrayT<Vertex2D>());
  int i_id_1 = mesh_buffer.Register(new IndexArray());
  int i_id_2 = mesh_buffer.Register(new IndexBufferObject());

  EXPECT_TRUE(v_id_1 < v_id_2);
  EXPECT_TRUE(i_id_1 < i_id_2);

  EXPECT_EQ(true, mesh_buffer.IsVertexBufferExist(v_id_1));
  EXPECT_EQ(true, mesh_buffer.IsVertexBufferExist(v_id_2));
  EXPECT_EQ(true, mesh_buffer.IsIndexBufferExist(i_id_1));
  EXPECT_EQ(true, mesh_buffer.IsIndexBufferExist(i_id_2));

  EXPECT_EQ(false, mesh_buffer.IsVertexBufferExist(v_id_2 + 100));
  EXPECT_EQ(false, mesh_buffer.IsIndexBufferExist(i_id_1 + 100));
}