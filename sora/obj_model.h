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

#pragma once
#include "globals.h"
#include "vertex.h"

namespace sora {;

class ObjModel;
class ObjWireFrameModel;

class ObjModel {
  friend class ObjWireFrameModel;
public:
  ObjModel();
  ~ObjModel();
  int vertex_count() const;
  int index_count() const;

  void AddVertex(const Vertex &v);
  void AddIndex(ushort idx);

  void OnLoadComplete();

  std::vector< DrawCmdData<Vertex> >::iterator Begin() { return draw_cmd_list_.begin(); }
  std::vector< DrawCmdData<Vertex> >::iterator End() { return draw_cmd_list_.end(); }
  const std::vector< DrawCmdData<Vertex> > &cmd_list() const { return draw_cmd_list_; }

private:
  std::vector<Vertex> vert_list_;
  // GL_TRIANGLES로 그릴수 잇도록 정렬된 인덱스 리스트
  std::vector<ushort> index_list_;
  std::vector<DrawCmdData<Vertex>> draw_cmd_list_;
};

class ObjWireFrameModel {
public:
  ObjWireFrameModel(const ObjModel &model);
  ~ObjWireFrameModel();

  std::vector< DrawCmdData<Vertex> >::iterator Begin() { return draw_cmd_list_.begin(); }
  std::vector< DrawCmdData<Vertex> >::iterator End() { return draw_cmd_list_.end(); }
  const std::vector< DrawCmdData<Vertex> > &cmd_list() const { return draw_cmd_list_; }

private:
  std::vector<DrawCmdData<Vertex>> draw_cmd_list_;
};

} //namespace sora
