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
#ifndef SORA_SELECTOR_H_
#define SORA_SELECTOR_H_

namespace sora {;
class UIComponent;

class Selector {
public:
  virtual void CallFunc() {}
  virtual void UICallback(UIComponent *obj) {}
};

typedef void (Selector::*Selector_CallFunc)();
typedef void (Selector::*Selector_UICallback)(UIComponent*);

struct CallFuncFuctor {
  CallFuncFuctor(Selector *target, Selector_CallFunc func) : target(target), func(func) {}
  void operator()() { (target->*func)(); }
  Selector *target;
  Selector_CallFunc func;
};

#define SR_CALL_FUNC_SEL(SELECTOR) (sora::Selector_CallFunc)(&SELECTOR)
#define SR_UI_CALLBACK_SEL(SELECTOR) (sora::Selector_UICallback)(&SELECTOR)
}

#endif  // SORA_SELECTOR_H_