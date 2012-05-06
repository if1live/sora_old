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

using namespace std;

namespace sora {;
ObjModel::ObjModel() {
}
ObjModel::~ObjModel() {
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
void ObjModel::OnLoadComplete() {
  DrawCmdData<Vertex> draw_cmd;
  draw_cmd.draw_mode = kDrawTriangles;
  draw_cmd.vertex_list = vert_list_;
  draw_cmd.index_list = index_list_;
  draw_cmd_list_.push_back(draw_cmd);
}

///////////////////////////////
ObjWireFrameModel::ObjWireFrameModel(const ObjModel &model) {
  vector<ushort> line_index_list;
  //lines 인덱스 구성하기
  for(size_t i = 0 ; i < model.index_list_.size() / 3 ; i++) {
    int idx1 = i*3 + 0;
    int idx2 = i*3 + 1;
    int idx3 = i*3 + 2;

    line_index_list.push_back(idx1);
    line_index_list.push_back(idx2);

    line_index_list.push_back(idx2);
    line_index_list.push_back(idx3);

    line_index_list.push_back(idx3);
    line_index_list.push_back(idx1);
  }

  DrawCmdData<Vertex> draw_cmd;
  draw_cmd.draw_mode = kDrawLines;
  draw_cmd.vertex_list = model.vert_list_;
  draw_cmd.index_list = line_index_list;
  draw_cmd_list_.push_back(draw_cmd);
}
ObjWireFrameModel::~ObjWireFrameModel() {
}

} //namespace sora