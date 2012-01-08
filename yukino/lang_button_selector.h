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
#ifndef YUKINO_LANG_BUTTON_SELECTOR_H_
#define YUKINO_LANG_BUTTON_SELECTOR_H_

#include "sora/selector.h"

namespace sora {;
class UIComponent;
class Button;
}

namespace yukino {;
class LangButtonSelector : public sora::Selector {
public:
  LangButtonSelector(sora::Button *eng_enable,
    sora::Button *eng_disable,
    sora::Button *kor_enable,
    sora::Button *kor_disable);
 LangButtonSelector();
  
  void Set(float ui_x, float ui_y);

  virtual void EngPressed(sora::UIComponent *obj);
  virtual void KorPressed(sora::UIComponent *obj);
  
  //button ptr
  sora::Button *eng_enable_btn;
  sora::Button *eng_disable_btn;
  sora::Button *kor_enable_btn;
  sora::Button *kor_disable_btn;
};
}

#endif  // YUKINO_LANG_BUTTON_SELECTOR_H_