/*  Copyright (C) 2011 by if1live */
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
#ifndef YUKINO_RENDERER_H_
#define YUKINO_RENDERER_H_

#include "sora/template_library.h"

namespace yukino {;
class RendererPimpl;

// position 3float + texcoord 2float 를 받아서 적절히 그리는거
// 기존 asuna포팅 및 대부분의 경우는 이거로 충분할듯함
// sora가 아니라 따로 둔 것은 이것은 라이브러리라고 하기에는 사용 용도가 너무 한정되니까
class Renderer : public sora::Singleton<Renderer> {
public:
  Renderer();
  ~Renderer();

  void Init();

  //immediate mode
  static void Begin(GLenum mode);
  static void Vertex3f(float x, float y, float z);
  static void Vertex2f(float x, float y) { Vertex3f(x, y, 0); }
  static void TexCoord2f(float s, float t);
  static void End();
private:
  RendererPimpl *impl_;
};
}

#define srglBegin       yukino::Renderer::Begin
#define srglEnd         yukino::Renderer::End
#define srglVertex3f    yukino::Renderer::Vertex3f
#define srglVertex2f    yukino::Renderer::Vertex2f
#define srglTexCoord2f  yukino::Renderer::TexCoord2f

#endif  // YUKINO_RENDERER_H_