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
#include "shader.h"

namespace sora {;

class PostEffect {
public:
  PostEffect();
  ~PostEffect();

  void Deinit();
  //post effect를 단계적으로 수행해야하는 경우 프레임버퍼를 적절히 써서 떄우자
  void Draw(Texture &tex, RenderDevice *dev);
  //tex를 화면 일부에 통쨰로 그릴떄 사용
  void Draw(Texture &tex, RenderDevice *dev, int x, int y, int w, int h);
  //화면의 일부에만 포스트 이펙트를 먹일경우에 사용한다
  //디버깅이라든가하는곳에서 쓸수잇을듯하다
  //좌표계는 scissor같은 느낌으로 왼쪽아래가 0, 0, 오르쪽위로가면 증가
  void DrawScissor(Texture &tex, RenderDevice *dev, int x, int y, int w, int h);
  void InitFromFile(const std::string &vert_path, const std::string &frag_path);

public:
  Shader post_effect_;
};

}