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