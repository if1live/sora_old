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
#ifndef SORA_MESH_H_
#define SORA_MESH_H_

#include "vertex.h"

namespace sora {;

struct DrawCommand;

//메시를 그리는데 필요한 정보. 렌더러는 이 정보를 얻어서 돌아가도록했다
struct DrawCommand {
  DrawCommand() 
    : draw_mode(GL_TRIANGLES), 
    vert_ptr(NULL),
    index_ptr(NULL), 
    index_count(0),
    index_type(GL_UNSIGNED_SHORT) { }

  GLenum draw_mode;
  const Vertex *vert_ptr;
  const void *index_ptr;
  int index_count;
  GLenum index_type;
};

}

#endif  // SORA_MESH_H_