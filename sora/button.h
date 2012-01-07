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
#ifndef SORA_BUTTON_H_
#define SORA_BUTTON_H_

#include "ui_component.h"

#include "texture_atlas.h"
#include "rect.h"

namespace sora {;

class ButtonHandler;

class Button : public UIComponent {
public:
	static UIComponent *Create(ButtonHandler *handler, const Recti &touch_area);
public:
	Button();
	virtual ~Button();

	void SetHandler(ButtonHandler *handler);

	//button state에 맞춰서 얻기
	TextureSubImage *GetImage();

	void OnPressed();
	void OnReleased();

  ButtonState button_state() const { return button_state_; }
	///@brief 터치 영역의 크기. 버튼의 좌표와 터치영역은 별개로 돌아간다
	///두개를 따로 둔 이유는 실제로 보이는영역과 터치 영역을 같게하면
	///버튼이 작을경우, 잘못누를 가능성이 높아지기 때문
	///귀찮아도 잘 지정하자
  void set_touch_rect(const Recti &rect) { touch_rect_ = rect; }
  const Recti &touch_rect() const { return touch_rect_; }

private:
	//friend class를 사용함으로써 외부노출을 완전히 차단(제한된 클래스외에는
	//버튼의 상태를 바꿀수없어야한다)
	friend class Container;
  void set_button_state(ButtonState state) { button_state_ = state; }

	//설마 버튼 1개에 기능이 여러개 붙진 않겠지??
	std::auto_ptr<ButtonHandler> handler_;
	ButtonState button_state_;
	Recti touch_rect_;

  TextureSubImage released_img_;
	TextureSubImage pressed_img_;
};

class ButtonHandler {
public:
	ButtonHandler() {}
	virtual ~ButtonHandler() {}
	virtual void OnPressed() = 0;
	virtual void OnReleased() = 0;
};

class TestButtonHandler : public ButtonHandler {
public:
	TestButtonHandler() {}
	virtual ~TestButtonHandler() {}
	virtual void OnPressed() { printf("pressed\n"); }
	virtual void OnReleased() { printf("released\n"); }
};
}

#endif