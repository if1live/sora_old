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
#include "ui_drawer.h"

#include "gl_inc.h"
#include "immediate_mode_emulator.h"

#include "image_label.h"
#include "button.h"
#include "ui_container.h"

#include "texture_atlas.h"
#include "texture_manager.h"
#include "texture.h"

#if SR_USE_PCH == 0
#include <boost/foreach.hpp>
#endif

namespace sora {;
void UIDrawer::Draw(TextureSubImage *img) {
  Texture *tex = TextureManager::GetInstance().GetTexture(img->tex_handle);
  if (tex == NULL) {
    return;
  }
  srglBindTexture(GL_TEXTURE_2D, tex->handle());

	//sub img의 크기를 사용해서 그리기
  float width = img->w;
  float height = img->h;

	//http://zarpar.tistory.com/tag/cocos2d
	//- texCoord.left = (rect.origin.x*2+1) / (texture.wide*2);
	//- texCoord.right = texCoord.left + (rect.size.width*2-2)/(texture.wide*2);
	//좌우로 약간 잘라서 그리기 떄문에 애니메이션이 100%동일하게 나오지 않는다
	//그래서 일단 주석해제하고 아래의 보정코드는 냅뒀다가 후에 필요해지면 다시 꺼내자
	float left, right, top, bottom;
	img->GetFrame(&left, &right, &bottom, &top);

  // ui는 y축 좌표계가 반대니까 raw gl좌표를 썡으로 그리면
  // 텍스쳐가 반대로 나와서 잘 나올거다

  srglScalef(width, height, 1);
  srglBegin(GL_QUADS);
  srglTexCoord2f(left, bottom);  srglVertex2f(-0.5f, -0.5f);
  srglTexCoord2f(right, bottom); srglVertex2f(0.5f, -0.5f);
  srglTexCoord2f(right, top);  srglVertex2f(0.5f, 0.5f);  
  srglTexCoord2f(left, top);   srglVertex2f(-0.5f, 0.5f);
  srglEnd();
}

void UIDrawer::Draw(ImageLabel *label) {
  if (!label->visible()) {
    return;
  }

	//적절히 그리기
	TextureSubImage &img = label->img();

	//image
  const float &win_width = win_width_;
  const float &win_height = win_height_;
	srglPushMatrix();
	const vec2 &pos = label->position();
  float w = img.w;
  float h = img.h;
	float x = w/2 + pos.x;
  float y = h/2 + pos.y;
	srglTranslatef(x, y, 0);

  Draw(&img);

	srglPopMatrix();
}

void UIDrawer::Draw(Button *btn) {
  if (!btn->visible()) {
    return;
  }

	TextureSubImage *img = btn->GetImage();
  const float &win_width = win_width_;
  const float &win_height = win_height_;

	if(img != NULL) {
		srglPushMatrix();
		const vec2 &pos = btn->position();
    float w = img->w;
		float h = img->h;

		float x = w/2 + pos.x;
    float y = h/2 + pos.y;
		srglTranslatef(x, y, 0);

    Draw(img);
		srglPopMatrix();
	}
}

void UIDrawer::BeforeDraw() {
	//glortho로 창 초기화하고 적절히 꽉 채워서 그리기
	srglMatrixMode(SR_MODELVIEW);
	srglPushMatrix();
	srglMatrixMode(SR_PROJECTION);
	srglPushMatrix();

	srglLoadIdentity();
  const float &win_w = win_width_;
  const float &win_h = win_height_;

  srglOrtho(0, win_w, win_h, 0, -100, 100);
  
  //modelview맞춰서 그리기
	srglMatrixMode(SR_MODELVIEW);
	srglLoadIdentity();
}

void UIDrawer::AfterDraw() {
	srglMatrixMode(SR_PROJECTION);
	srglPopMatrix();
	srglMatrixMode(SR_MODELVIEW);
	srglPopMatrix();
}


void UIDrawer::DrawTouchArea(UIContainer *container) {
	BeforeDraw();

  const float &win_width = win_width_;
  const float &win_height = win_height_;

	UIContainer::ButtonListType btnlist;
	container->GetButtonList(btnlist);
	BOOST_FOREACH(Button *btn, btnlist) {
    const Recti &area = btn->GetTouchRect();

		float x = area.origin().x;
		float y = area.origin().y;
		float w = area.width();
		float h = area.height();

		float center_x = x + w/2;
		float center_y = y + h/2;

    // 순수하게 색만 쓰고싶으면 텍스쳐를 교체해야한다
    // 왜냐하면 즉시모드 에뮬에는 tex끄기가 아직 없어서...
    if(btn->button_state() == kButtonReleased) {
			srglBindTexture(GL_TEXTURE_2D, Texture::Red().handle());
		} else {
      srglBindTexture(GL_TEXTURE_2D, Texture::Green().handle());
		}
    
    srglBegin(GL_LINE_LOOP);
    srglVertex2f(center_x - w/2, center_y - h/2);
    srglVertex2f(center_x + w/2, center_y - h/2);
    srglVertex2f(center_x + w/2, center_y + h/2);
    srglVertex2f(center_x - w/2, center_y + h/2);
    srglEnd();
	}

	AfterDraw();
}

void UIDrawer::DrawRoot(UIComponent *root) {
  BeforeDraw();

  // double dispatch로 구현
  root->Draw(this);

	//행렬 복구
	AfterDraw();
}

void UIDrawer::Draw(UIComponent *comp) {
  if (comp->visible() == false) {
    return;
  }

  srglPushMatrix();

  const float &win_height = win_height_;
	
	const vec2 &pos = comp->position();
  //container는 크기라는게 미묘하니 왼쪽위를 그냥 쓰자
	srglTranslatef(pos.x, pos.y, 0);

  for (size_t i = 0 ; i < comp->ChildCount() ; i++) {
    UIComponent *elem = comp->GetChild(i);
		UIComponentType comp_type = elem->ui_component_type();
    switch(comp_type) {
    case kImageLabel:
			Draw(elem->Cast<ImageLabel>());
      break;
    case kButton:
			Draw(elem->Cast<Button>());
      break;
    case kUIContainer:
      Draw(elem->Cast<UIContainer>());
      break;
    default:
      SR_ASSERT(!"todo impl");
      break;
    }
  }

	srglPopMatrix();
}
}