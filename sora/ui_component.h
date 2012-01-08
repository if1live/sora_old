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
#include "input_enum.h"
#include "touch.h"
#include "rect.h"

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
class UIComponent : boost::noncopyable, public TouchListener {
public:
  template<typename T>
	static bool IsContain(const Rect<T> &area, const Vector2<T> &touch);

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

  // for touch
public:
  virtual void TouchBegan(const TouchDevice::EventListType &evt_list) {}
  virtual void TouchMoved(const TouchDevice::EventListType &evt_list) {}
  virtual void TouchEnded(const TouchDevice::EventListType &evt_list) {}
  virtual void TouchCancelled(const TouchDevice::EventListType &evt_list) {}

public:
  //state
  UIComponentType ui_component_type() const { return ui_component_type_; }
  bool visible() const { return visible_; }
  void set_visible(bool v) { visible_ = v; }
  const vec2 &position() const { return position_; }
  void set_position(const vec2 &pos) { position_ = pos; }
  void set_position(float ui_x, float ui_y) { position_.x = ui_x; position_.y = ui_y; }

  // 절대좌표얻기. 부모를 따라가면서 부모의 위치까지 적용된 좌표
  vec2 GetAbsolutePosition();
  
  // enable 상태에 의해서 로직이 아예 진입 안할수도 있다
  bool is_enable() const { return is_enable_; }
  void set_is_enable(bool e) { is_enable_ = e; }
  bool Enable() { is_enable_ = true; }
  bool Disable() { is_enable_ = false; }

  const Color4ub &color() const { return color_; }
  void set_color(const Color4ub &c) { color_ = c; }

  UIComponent *parent() { return parent_; }
  void set_parent(UIComponent *parent) { parent_ = parent; }
private:
	UIComponentType ui_component_type_;
  bool visible_;
  bool is_enable_;
	//ui 객체를 그릴때의 왼쪽위의 좌표
	//view와 관계있고 터치와는 무관함
	sora::vec2 position_;
  Color4ub color_;

  UIComponent *parent_;

public:
  // 상속없이 일부 추가 데이터를 쓰기 위한 용도
  void set_userdata(void *data) { data_ = data; }
  void *userdata() { return data_; }
private:
  void *data_;
};

template<typename T>
bool UIComponent::IsContain(const sora::Rect<T> &area, const sora::Vector2<T> &touch) {
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
#endif  // SORA_UI_COMPONENT_H_