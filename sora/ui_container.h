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
#include "rect.h"

namespace sora {;
class Button;

template<typename T>  class Rect;
template<typename T>  class Vector2;

class UIContainer : public UIComponent {
public:
	template<typename T>
	static bool IsContain(const sora::Rect<T> &area, const sora::Vector2<T> &touch);
	typedef std::vector<Button*> ButtonListType;

public:
	UIContainer();
	~UIContainer();

	virtual void Add(UIComponent *comp);
  virtual int ChildCount() const { return comp_list_.size(); }
  virtual UIComponent *GetChild(int index);

	//디버깅용으로 터치되는 보이는 영역 보이기
	//void DrawTouchArea();
	//void Update(float dt, const touch::TouchEventList &evt_list);

	void GetButtonList(ButtonListType &out);

private:
	void GetVisibleButtonList(ButtonListType &out);

	std::vector<UIComponent *> comp_list_;
};

}	// namespace

namespace sora {;
template<typename T>
bool UIContainer::IsContain(const sora::Rect<T> &area, const sora::Vector2<T> &touch) {
	//좌표계가 왼쪽위가 0,0인것을 고려해서 잘 계산
	T x = area.origin().x;
	T y = area.origin().y;
	T w = area.width();
	T h = area.height();
		
	//x방향은 좌표문제없으니까 일단 확인
	if(x > touch.x || x+w < touch.x) {
		return false;
	}
	if(y > touch.y || y+h < touch.y) {
		return false;
	}
	return true;
}
}
#endif  // SORA_UI_CONTAINER_H_