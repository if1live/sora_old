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
ObjModel::ObjModel() {
}
ObjModel::~ObjModel() {
}

std::vector< DrawCommand<Vertex> > ObjModel::GetDrawCmdList_wire() const {
  //lines 인덱스 구성하기
  if(line_index_list_.empty()) {
    for(size_t i = 0 ; i < index_list_.size() / 3 ; i++) {
      int idx1 = i*3 + 0;
      int idx2 = i*3 + 1;
      int idx3 = i*3 + 2;

      line_index_list_.push_back(idx1);
      line_index_list_.push_back(idx2);

      line_index_list_.push_back(idx2);
      line_index_list_.push_back(idx3);

      line_index_list_.push_back(idx3);
      line_index_list_.push_back(idx1);
    }
  }

  vector< DrawCommand<Vertex> > cmd_list;
  DrawCommand<Vertex> draw_cmd;
  draw_cmd.vert_ptr = vertex_ptr();
  draw_cmd.draw_mode = GL_LINES;
  draw_cmd.index_ptr = &line_index_list_[0];
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.index_count = line_index_list_.size();
  draw_cmd.vert_count = vertex_count();
  cmd_list.push_back(draw_cmd);
  return cmd_list;
}

const Vertex *ObjModel::vertex_ptr() const {
  return &vert_list_[0];
}
const void *ObjModel::index_ptr() const {
  return &index_list_[0];
}
int ObjModel::vertex_count() const {
  return vert_list_.size();
}
int ObjModel::index_count() const {
  return index_list_.size();
}

void ObjModel::AddVertex(const Vertex &v) {
  vert_list_.push_back(v);
}
void ObjModel::AddIndex(ushort idx) {
  index_list_.push_back(idx);
}

}
