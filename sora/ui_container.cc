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
#include "ui_container.h"

#include "template_library.h"
#include "ui_drawer.h"

#include "image_label.h"
#include "button.h"

namespace sora {;
//drawer 교체는 아직 생각없음
UIDrawer drawer;

UIContainer::UIContainer()
  : UIComponent(kUIContainer) {
}
UIContainer::~UIContainer() {
	DestroyList(&comp_list_);
}
void UIContainer::Draw(UIDrawer *drawer) {
  drawer->Draw(this);
}
void UIContainer::Add(UIComponent *comp) {
	comp_list_.push_back(comp);
  comp->set_parent(this);
}
UIComponent *UIContainer::GetChild(int index) {
  if (index >= 0 && index < ChildCount()) {
    return comp_list_[index];
  } else {
    return NULL;
  }
}

#if 0
void Container::Update(float dt, const touch::TouchEventList &evt_list) {
	if(evt_list.IsEmpty()) { 
		return;
	}

	//보이는 버튼 목록 얻기
	//안보이는 버튼은 못누름
	vector<Button*> btnlist;
	GetVisibleButtonList(btnlist);
	//현재 눌려져있는 좌표 목록
	vector<ivec2> touchPoslist;	//눌려져있는 좌표 목록
	touchPoslist.reserve(16);
	BOOST_FOREACH(const TouchEvent &touch, evt_list.began_event_list()) {
		int x = touch.curr_x;
		int y = touch.curr_y;
		touchPoslist.push_back(ivec2(x, y));
	}
	BOOST_FOREACH(const TouchEvent &touch, evt_list.moved_event_list()) {
		int x = touch.curr_x;
		int y = touch.curr_y;
		touchPoslist.push_back(ivec2(x, y));
	}
	BOOST_FOREACH(const TouchEvent &touch, evt_list.ended_event_list()) {
		int x = touch.curr_x;
		int y = touch.curr_y;
		touchPoslist.push_back(ivec2(x, y));
	}
	BOOST_FOREACH(const TouchEvent &touch, evt_list.cancelled_event_list()) {
		int x = touch.curr_x;
		int y = touch.curr_y;
		touchPoslist.push_back(ivec2(x, y));
	}

	//begin list얻기
	vector<ivec2> beganlist;
	BOOST_FOREACH(const TouchEvent &touch, evt_list.began_event_list()) {
		int x = touch.curr_x;
		int y = touch.curr_y;
		beganlist.push_back(ivec2(x, y));
	}

	//ended list얻기
	vector<ivec2> endedlist;
	BOOST_FOREACH(const TouchEvent &touch, evt_list.ended_event_list()) {
		int x = touch.curr_x;
		int y = touch.curr_y;
		endedlist.push_back(ivec2(x, y));
	}

	//눌려져있는 좌표가 아님+버튼이 눌린상태=>안눌린 상태로
	BOOST_FOREACH(Button *btn, btnlist) {
		BOOST_FOREACH(const ivec2 &pos, touchPoslist) {
			if(IsContain(btn->touch_rect(), pos) == false && btn->button_state() == kButtonPressed) {
				btn->OnReleased();
				btn->set_button_state(kButtonReleased);
			}
		}
	}

	//눌리는 순간+버튼이 안눌링 상태=>눌린 상태
	BOOST_FOREACH(Button *btn, btnlist) {
		BOOST_FOREACH(const ivec2 &pos, beganlist) {
			if(IsContain(btn->touch_rect(), pos) == true && btn->button_state() == kButtonReleased) {
				btn->OnPressed();
				btn->set_button_state(kButtonPressed);
			}
		}
	}
	/*
	//눌려져있는 버튼+누른터치 좌표없음=>release
	if(touchPoslist.size() == 0) {
	BOOST_FOREACH(Button *btn, btnlist) {
	if(btn->button_state() == kButtonPressed) {
	btn->OnReleased();
	btn->set_button_state(kButtonReleased);
	}
	}
	}
	*/
	//눌려져있는 버튼+ended가 버튼 위에서 발생=>release
	BOOST_FOREACH(Button *btn, btnlist) {
		BOOST_FOREACH(const ivec2 &pos, endedlist) {
			if(IsContain(btn->touch_rect(), pos) == true && btn->button_state() == kButtonPressed) {
				btn->OnReleased();
				btn->set_button_state(kButtonReleased);
			}
		}
	}
}
#endif

void UIContainer::GetButtonList(ButtonListType &out) {
	SR_ASSERT(out.empty() == true);
	//버튼 목록 얻기
	BOOST_FOREACH(UIComponent *comp, comp_list_) {
		UIComponentType comp_type = comp->ui_component_type();
		if(comp_type == kButton) {
			Button *btn = comp->Cast<Button>();
			out.push_back(btn);
		}
	}
}
void UIContainer::GetVisibleButtonList(ButtonListType &out) {
	SR_ASSERT(out.empty() == true);
	//버튼 목록 얻기
	BOOST_FOREACH(UIComponent *comp, comp_list_) {
		if(comp->visible() == false) {
			continue;
		}
		UIComponentType comp_type = comp->ui_component_type();
		if(comp_type == kButton) {
			Button *btn = comp->Cast<Button>();
			out.push_back(btn);
		}
	}
}

}