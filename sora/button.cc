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

namespace sora {;
Button::Button()
	: UIComponent(kButton),
	button_state_(kButtonReleased) {
}
Button::Button(const TextureSubImage &img)
  : UIComponent(kButton),
	button_state_(kButtonReleased),
  released_img_(img), pressed_img_(img) {
}
Button::~Button() {
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
}
