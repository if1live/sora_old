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
#include "sora_stdafx.h"
#include "obj_model.h"

#include <boost/tokenizer.hpp>

using namespace std;

namespace sora {;
struct ObjModelImpl {;
  std::vector<Vertex> vert_list;
  // GL_TRIANGLES로 그릴수 잇도록 정렬된 인덱스 리스트
  std::vector<ushort> index_list;
};

ObjModel::ObjModel()
: impl(new ObjModelImpl()) {
}
ObjModel::~ObjModel() {
  delete(impl);
}

const Vertex *ObjModel::vertex_ptr() const {
  return &impl->vert_list[0];
}
const void *ObjModel::index_ptr() const {
  return &impl->index_list[0];
}
int ObjModel::vertex_count() const {
  return impl->vert_list.size();
}
int ObjModel::index_count() const {
  return impl->index_list.size();
}

void ObjModel::AddVertex(const Vertex &v) {
  impl->vert_list.push_back(v);
}
void ObjModel::AddIndex(ushort idx) {
  impl->index_list.push_back(idx);
}

}
