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
#ifndef SORA_UICOMPONENT_H_
#define SORA_UICOMPONENT_H_

namespace sora {;
// Composite pattern 으로 UI 시스템 구축
class UIComponent {
public:
  UIComponent() : visible(true), enable(true) {}
  virtual ~UIComponent() {}

  virtual void Add(UIComponent *child) {}

public:
  // 어차피 getter/setter전부 뚫을 속성은 public으로 두자
  bool visible;
  bool enable;
};

class Button : public UIComponent {
public:
  Button() {}
  virtual ~Button() {}
};

class ImageLabel : public UIComponent {
public:
  ImageLabel() {}
  virtual ~ImageLabel() {}
};

class UIContainer : public UIComponent {
public:
  UIContainer() {}
  virtual ~UIContainer() {}

  virtual void Add(UIComponent *child);
private:
  std::vector<UIComponent*> comp_list_;
};
}

#endif  // SORA_UICOMPONENT_H_