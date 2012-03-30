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
#ifndef SORA_RUNA_VIEW_H_
#define SORA_RUNA_VIEW_H_

//하나의 독립된 프로그램으로써 material editor를 만들기로 햇다
//코드명은 RUNA
#include "gl_view.h"

namespace sora {;
struct RunaViewPrivate;

public ref class RunaView : public GLView {
public:
  RunaView();
  ~RunaView();
  virtual void SetupGraphics(int w, int h) override;
  virtual void SetWindowSize(int w, int h) override;
  virtual void DrawFrame() override;
  virtual void InitGLEnv() override;
  virtual void UpdateFrame(float dt) override;
  virtual void Cleanup() override;
private:
  RunaViewPrivate &pimpl();
  RunaViewPrivate *pimpl_;
};
}


#endif  // SORA_RUNA_VIEW_H_