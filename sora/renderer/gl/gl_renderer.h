﻿/*  Copyright (C) 2011-2012 by if1live */
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
#ifndef SORA_GL_DEFERRED_RENDERER_H_
#define SORA_GL_DEFERRED_RENDERER_H_

#include "renderer/globals.h"
#include "core/vertex.h"

namespace sora {;
namespace gl {
  class GLRenderer {
  public:
    GLRenderer();
    ~GLRenderer();

    template<typename VertexType>
    void DrawArrays(DrawType mode, const std::vector<VertexType> &vert_list);
    template<typename VertexType>
    void DrawElements(DrawType mode, const std::vector<VertexType> &vert_list, const IndexList &index_list);
  private:
  };

  //impl
  template<typename VertexType>
  void GLRenderer::DrawArrays(DrawType mode, const std::vector<VertexType> &vert_list) {
  }
  template<typename VertexType>
  void GLRenderer::DrawElements(DrawType mode, const std::vector<VertexType> &vert_list, const IndexList &index_list) {

  }

} // namespace gl
} // namespace sora

#endif  // SORA_GL_DEFERRED_RENDERER_H_