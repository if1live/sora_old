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
#ifndef SORA_UI_COMPONENT_H_
#define SORA_UI_COMPONENT_H_

#include "color.h"

namespace sora {;
class UIContainer;
class UIComponent;
class ImageLabel;
class Button;
class UIDrawer;

typedef enum {
	kImageLabel,
	kButton,
  kUIContainer,
} UIComponentType;

typedef enum {
	kButtonPressed,
	kButtonReleased
} ButtonState;

//ui의 좌표계는 왼쪽위가 0,0이다
//터치 좌표계와 동일하게 맞춰서 변환 삽질을 줄였다
//ui system을 사용하기전에 touch queue을 작동시켜놓는다

// ui좌표계는 왼쪽위가 0,0이다
// 일반적으로 사용되는 GUI와 같은 좌표계를 채택했다
// 레티나 지원을 한다. 아이폰3gs와 아이폰4모두 480*320기준으로 돌아간다
// 안드로이드의 경우는 레티나가 없어 해상도가 곧 ui좌표계이다
// web의 css의 컨셉을 받아들여 9방향 배치가 가능하다
// 9방향 배치를 쓰면 해상도가 변해도 UI가 심각하지 붕괴하진 않을것이다

///ui는 멀티터치를 지원하지 않는다(굳이 필요하지도 않으니까)
///모든 ui component는 크기+위치를 표현하는 rect를 소유한다
class UIComponent : boost::noncopyable {
public:
	UIComponent(UIComponentType comp_type);
	virtual ~UIComponent();

	template<typename T>
	T *Cast() { return static_cast<T*>(this); }

public:
  virtual void Add(UIComponent *comp) { }
  virtual int ChildCount() const { return 0; }
  virtual UIComponent *GetChild(int index) { return NULL; }

  // visiter pattern based
  virtual void Draw(UIDrawer *drawer) = 0;

public:
  //state
  UIComponentType ui_component_type() const { return ui_component_type_; }
  bool visible() const { return visible_; }
  void set_visible(bool v) { visible_ = v; }
  const vec2 &position() const { return position_; }
  void set_position(const vec2 &pos) { position_ = pos; }
  void set_position(float ui_x, float ui_y) { position_.x = ui_x; position_.y = ui_y; }
  
  // enable 상태에 의해서 로직이 아예 진입 안할수도 있다
  bool is_enable() const { return is_enable_; }
  void set_is_enable(bool e) { is_enable_ = e; }
  bool Enable() { is_enable_ = true; }
  bool Disable() { is_enable_ = false; }

  const Color4ub &color() const { return color_; }
  void set_color(const Color4ub &c) { color_ = c; }

private:
	UIComponentType ui_component_type_;
  bool visible_;
  bool is_enable_;
	//ui 객체를 그릴때의 왼쪽위의 좌표
	//view와 관계있고 터치와는 무관함
	sora::vec2 position_;

  Color4ub color_;
};
}
#endif  // SORA_UI_COMPONENT_H_