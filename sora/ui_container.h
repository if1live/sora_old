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
#ifndef SORA_UI_CONTAINER_H_
#define SORA_UI_CONTAINER_H_
#include "ui_component.h"

namespace sora {;
class Button;
class UIDrawer;
class UIContainer : public UIComponent {
public:
	typedef std::vector<Button*> ButtonListType;

public:
	UIContainer();
	~UIContainer();

	virtual void Add(UIComponent *comp);
  virtual int ChildCount() const { return comp_list_.size(); }
  virtual UIComponent *GetChild(int index);

  virtual void Draw(UIDrawer *drawer);

	//디버깅용으로 터치되는 보이는 영역 보이기
	//void Update(float dt, const touch::TouchEventList &evt_list);

	void GetButtonList(ButtonListType &out);

private:
	void GetVisibleButtonList(ButtonListType &out);

	std::vector<UIComponent *> comp_list_;
};

}	// namespace

#endif  // SORA_UI_CONTAINER_H_