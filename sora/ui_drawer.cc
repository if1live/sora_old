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

#include "gl_window.h"

#include "gl_inc.h"
#include "immediate_mode_emulator.h"

#include "image_label.h"
#include "button.h"

#include "texture_atlas.h"
#include "texture_manager.h"
#include "texture.h"

namespace sora {;
void UIDrawer::Draw(TextureSubImage *img) {
  Texture *tex = TextureManager::GetInstance().GetTexture(img->tex_handle);
  srglBindTexture(GL_TEXTURE_2D, tex->handle);

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

  srglScalef(width, height, 1);
  srglBegin(GL_QUADS);
  srglTexCoord2f(left, top);  srglVertex2f(-0.5f, -0.5f);
  srglTexCoord2f(right, top); srglVertex2f(0.5f, -0.5f);
  srglTexCoord2f(right, bottom);  srglVertex2f(0.5f, 0.5f);  
  srglTexCoord2f(left, bottom);   srglVertex2f(-0.5f, 0.5f);
  srglEnd();
}

void UIDrawer::Draw(ImageLabel *label) {
	//적절히 그리기
	TextureSubImage &img = label->img();

	//image
  float win_width = GLWindow::GetInstance().width();
  float win_height = GLWindow::GetInstance().height();
	srglPushMatrix();
	const vec2 &pos = label->position();
  float w = img.w;
  float h = img.h;

	float x = w/2 + pos.x;
  float y = win_height - pos.y - h/2;
	srglTranslatef(x, y, 0);

  Draw(&img);

	srglPopMatrix();
}

void UIDrawer::Draw(Button *btn) {
	TextureSubImage *img = btn->GetImage();
  float win_width = GLWindow::GetInstance().width();
  float win_height = GLWindow::GetInstance().height();

	if(img != NULL) {
		srglPushMatrix();
		const vec2 &pos = btn->position();
    float w = img->w;
		float h = img->h;

		float x = w/2 + pos.x;
    float y = win_height - pos.y - h/2;
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
  float win_w = GLWindow::GetInstance().width();
  float win_h = GLWindow::GetInstance().height();

  srglOrtho(0, win_w, 0, win_h, -100, 100);
  
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

/*
void UIDrawer::DrawTouchArea(Container *container) {
	BeforeDraw();

	vec2 winsize = Device::GetInstance().GetScreenSize();
	Container::ButtonListType btnlist;
	container->GetButtonList(btnlist);
	BOOST_FOREACH(Button *btn, btnlist) {
		const Recti &area = btn->touch_rect();

		float x = area.origin().x;
		float y = area.origin().y;
		float w = area.width();
		float h = area.height();

		float centerX = x + w/2;
		float centerY = winsize.y - y - h/2;

		Color4ub color;
		if(btn->visible) {
			ToRed(color);
		} else {
			ToGreen(color);
		}
		DrawHelper::DrawLineRect(vec2(centerX, centerY), w, h, color);
	}

	AfterDraw();
}
*/
void UIDrawer::DrawRoot(UIComponent *root) {
  BeforeDraw();

  for (size_t i = 0 ; i < root->ChildCount() ; i++) {
    UIComponent *comp = root->GetChild(i);
		if(comp->visible() == false) {
			continue;
		}

		UIComponentType comp_type = comp->ui_component_type();
    switch(comp_type) {
    case kImageLabel:
			Draw(comp->Cast<ImageLabel>());
      break;
    case kButton:
			Draw(comp->Cast<Button>());
      break;
    default:
      SR_ASSERT(!"todo impl");
      break;
    }
	}

	//행렬 복구
	AfterDraw();
}
}