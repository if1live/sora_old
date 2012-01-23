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

#include "sora/render/texture_atlas.h"
#include "sora/rect.h"
#include "sora/selector.h"

namespace sora {;
class Button : public UIComponent {
public:
	Button();
  Button(const TextureSubImage &img);
	virtual ~Button();

  virtual void Draw(UIDrawer *drawer);


	void OnPressed();
	void OnReleased();

  ButtonState button_state() const { return button_state_; }
private:
	ButtonState button_state_;

  //터치 영역은 수동으로 잡아주면 그것을 쓰고, 없으면 이미지크기르 그냥 쓴다
public:
  ///@brief 터치 영역의 크기. 버튼의 좌표와 터치영역은 별개로 돌아간다
	///두개를 따로 둔 이유는 실제로 보이는영역과 터치 영역을 같게하면
	///버튼이 작을경우, 잘못누를 가능성이 높아지기 때문
	///귀찮아도 잘 지정하자
  void set_touch_rect(const Recti &rect) { touch_rect_ = rect; }
  Recti GetTouchRect();
  void ResetTouchRect();
private:
  Recti touch_rect_;

  
  //어떻게 그릴것인가?
public:
	TextureSubImage *GetImage();  //button state에 맞춰서 얻기
  void set_released_img(const TextureSubImage &img) { released_img_ = img; }
  void set_pressed_img(const TextureSubImage &img) { pressed_img_ = img; }
  void SetImage(const TextureSubImage &img) {
    released_img_ = img;  pressed_img_ = img;
  }
private:
  TextureSubImage released_img_;
	TextureSubImage pressed_img_;

  //event, 버튼에 이벤트가 발생할떄 처리할 기능 연결시키기
public:
  void set_pressed_functor(const UICallbackFunctor &f) { pressed_functor_ = f; }
  void set_released_functor(const UICallbackFunctor &f) { released_functor_ = f; }
private:
  UICallbackFunctor pressed_functor_;
  UICallbackFunctor released_functor_;

  // 터치 이벤트 처리하기
public:
  virtual void TouchBegan(const TouchDevice::EventListType &evt_list);
  virtual void TouchMoved(const TouchDevice::EventListType &evt_list);
  virtual void TouchEnded(const TouchDevice::EventListType &evt_list);
  virtual void TouchCancelled(const TouchDevice::EventListType &evt_list);
};
}

#endif