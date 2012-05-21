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

namespace sora {;

class PostEffect {
public:
  PostEffect();
  ~PostEffect();

  void Deinit();

  //post effect를 단계적으로 수행해야하는 경우 프레임버퍼를 적절히 써서 떄우자
  //Draw계열은 단순히 텍스쳐 1개를 통으로 그릴경우에 유효하다
  void Draw(Texture &tex);
  //tex를 화면 일부에 통쨰로 그릴떄 사용
  void Draw(Texture &tex, int x, int y, int w, int h);
  //화면의 일부에만 포스트 이펙트를 먹일경우에 사용한다
  //디버깅이라든가하는곳에서 쓸수잇을듯하다
  //좌표계는 scissor같은 느낌으로 왼쪽아래가 0, 0, 오르쪽위로가면 증가
  void DrawScissor(Texture &tex, int x, int y, int w, int h);
  void InitFromFile(const std::string &vert_path, const std::string &frag_path);
  void InitFromFile(const std::string &frag_path);

  Shader &post_effect();
  //포스트 이펙트 속성을 수동으로 쓸 경우, 내부 구현이 아니라 밖에서 수행하기 위해서
  //포스트이펙트 on/off를 외부에서 조정할수 잇도록햇다. 일반적으로 안써도됨
  void BeginPass();
  void EndPass();
  //post effect를 수동으로 조절할 경우, 그리기만 수행할 함수를 따로 둿다. 밑레벨 함수니까
  //수동호출은 자제하자. 이걸 쓸 경우, begin/end를 수동으로 호출해야한다
  void LowLevelDraw(Texture &tex);
  void LowLevelDraw(Texture &tex, int x, int y, int w, int h);
  void LowLevelDrawScissor(Texture &tex, int x, int y, int w, int h);

public:
  std::unique_ptr<Shader> post_effect_;
};

}