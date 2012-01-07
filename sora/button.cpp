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

namespace sora {;
UIComponent *Button::Create(ButtonHandler *handler, const Recti &touch_area) {
	Button *btn = new Button();
	btn->SetHandler(handler);
	btn->set_touch_rect(touch_area);
	return btn;
}
Button::Button()
	: UIComponent(kButton),
	button_state_(kButtonReleased) {
}
Button::~Button() {
}

void Button::SetHandler(ButtonHandler *handler) {
	handler_.reset(handler);
}
void Button::OnPressed() {
	if(handler_.get() != NULL) {
		handler_->OnPressed();
	}
}
void Button::OnReleased() {
	if(handler_.get() != NULL) {
		handler_->OnReleased();
	}
}
TextureSubImage *Button::GetImage() {
	if(button_state() == kButtonPressed) {
		return &pressed_img_;
	} else {
		return &released_img_;
	}
}
}
