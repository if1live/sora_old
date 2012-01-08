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
#include "sora_stdafx.h"
#include "button.h"
#include "ui_drawer.h"
#include "touch.h"

namespace sora {;
Button::Button()
: UIComponent(kButton),
button_state_(kButtonReleased) {
  TouchDevice::GetInstance().AddListener(this);
}
Button::Button(const TextureSubImage &img)
: UIComponent(kButton),
button_state_(kButtonReleased),
released_img_(img), pressed_img_(img) {
  TouchDevice::GetInstance().AddListener(this);
}
Button::~Button() {
  TouchDevice::GetInstance().RemoveListener(this);
}

void Button::Draw(UIDrawer *drawer) {
  drawer->Draw(this);
}
void Button::OnPressed() {
  pressed_functor_(this);
}
void Button::OnReleased() {
  released_functor_(this);
}
TextureSubImage *Button::GetImage() {
	if(button_state() == kButtonPressed) {
		return &pressed_img_;
	} else {
		return &released_img_;
	}
}
void Button::ResetTouchRect() {
  touch_rect_ = Recti();
}
Recti Button::GetTouchRect() {
  if (touch_rect_.IsEmpty()) {
    // 이미지 영역과 동일
    TextureSubImage *img = GetImage();
    float w = img->w;
    float h = img->h;
    float x = position().x;
    float y = position().y;

    return Recti(x, y, w, h);
  } else {
    return touch_rect_;
  }
}

// 터치 이벤트 처리는 각각의 컴포넌트마다 알아서 수행하자
void Button::TouchBegan(const TouchDevice::EventListType &evt_list) {
  if (!visible() || !is_enable()) {
    return;
  }

  // 일단 싱글터치만 취급
  const TouchEvent &evt = evt_list[0];
  Recti touch_area = GetTouchRect();
   
  // 버튼 위에서 이벤트가 시작되면
  if (IsContain(touch_area, ivec2(evt.curr_x, evt.curr_y))) {
    pressed_functor_(this);
    button_state_ = kButtonPressed;
  }
}
void Button::TouchMoved(const TouchDevice::EventListType &evt_list) {
  if (!visible() || !is_enable()) {
    return;
  }

  const TouchEvent &evt = evt_list[0];
  Recti touch_area = GetTouchRect();

  // 눌린상태 + 현재위치=밖->터치 탈출
  bool in_area = IsContain(touch_area, ivec2(evt.curr_x, evt.curr_y));
  if (button_state() == kButtonPressed && !in_area) {
    button_state_ = kButtonReleased;
  }
}
void Button::TouchEnded(const TouchDevice::EventListType &evt_list) {
  if (!visible() || !is_enable()) {
    return;
  }

  const TouchEvent &evt = evt_list[0];
  Recti touch_area = GetTouchRect();

  if (button_state_ == kButtonPressed 
    && IsContain(touch_area, ivec2(evt.curr_x, evt.curr_y))) {
    // 버튼 위에서 이벤트가 끝나면..
    released_functor_(this);
    button_state_ = kButtonReleased;
  }
  
  //어쩃든 버튼은 놓는다
  button_state_ = kButtonReleased;
}

void Button::TouchCancelled(const TouchDevice::EventListType &evt_list) {
  if (!visible() || !is_enable()) {
    return;
  }

  button_state_ = kButtonReleased;
}
}
