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
#ifndef SORA_OBJ_MODEL_H_
#define SORA_OBJ_MODEL_H_

#include "vertex.h"
namespace sora {;

struct ObjModel;
struct ObjModelImpl;
struct SR_DLL ObjModel {
  ObjModel();
  ~ObjModel();
  ObjModel(const ObjModel &o);
  ObjModel& operator=(const ObjModel &o);

  const Vertex *vertex_ptr() const;
  const void *index_ptr() const;
  int vertex_count() const;
  int index_count() const;

  void AddVertex(const Vertex &v);
  void AddIndex(ushort idx);

private:
  ObjModelImpl *impl;
};
}

#endif  // SORA_OBJ_MODEL_H_