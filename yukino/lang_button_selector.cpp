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
#include "yukino_stdafx.h"
#include "lang_button_selector.h"

#include "sora/ui/button.h"
#include "sora/locale.h"
#include "sora/render/sprite_sheet_manager.h"

using namespace sora;

namespace yukino {;
LangButtonSelector::LangButtonSelector(Button *eng_enable,
  Button *eng_disable,
  Button *kor_enable,
  Button *kor_disable)
: eng_enable_btn(eng_enable),
  eng_disable_btn(eng_disable),
  kor_enable_btn(kor_enable),
  kor_disable_btn(kor_disable) {
}
LangButtonSelector::LangButtonSelector()
: eng_enable_btn(NULL),
  eng_disable_btn(NULL),
  kor_enable_btn(NULL),
  kor_disable_btn(NULL) {
}

void LangButtonSelector::EngPressed(sora::UIComponent *obj) {
  eng_enable_btn->set_visible(true);
  eng_disable_btn->set_visible(false);
  kor_enable_btn->set_visible(false);
  kor_disable_btn->set_visible(true);
  Locale::GetInstance().SetUSA();
}
void LangButtonSelector::KorPressed(sora::UIComponent *obj) {
  eng_enable_btn->set_visible(false);
  eng_disable_btn->set_visible(true);
  kor_enable_btn->set_visible(true);
  kor_disable_btn->set_visible(false);

  Locale::GetInstance().SetKorea();
}

void LangButtonSelector::Set(float ui_x, float ui_y) {
  SpriteSheetManager &sprite_mgr = SpriteSheetManager::GetInstance();
  TextureSubImage *eng_enable_img = sprite_mgr.GetSubImage("BtMenuEnglish@2x");
  TextureSubImage *eng_disable_img = sprite_mgr.GetSubImage("BtMenuEnglishD@2x");
  TextureSubImage *kor_enable_img = sprite_mgr.GetSubImage("BtMenuKorean@2x");
  TextureSubImage *kor_disable_img = sprite_mgr.GetSubImage("BtMenuKoreanD@2x");

  SR_ASSERT(eng_disable_img != NULL);
  SR_ASSERT(eng_enable_img != NULL);
  SR_ASSERT(kor_disable_img != NULL);
  SR_ASSERT(kor_enable_img != NULL);

  eng_enable_btn->SetImage(*eng_enable_img);
  eng_disable_btn->SetImage(*eng_disable_img);
  kor_enable_btn->SetImage(*kor_enable_img);
  kor_disable_btn->SetImage(*kor_disable_img);

  // TODO 위치??
  vec2 kor_pos(ui_x + 0, ui_y + 0);
  vec2 eng_pos(ui_x + eng_enable_img->w, ui_y + 0);

  eng_disable_btn->set_position(eng_pos);
  eng_enable_btn->set_position(eng_pos);
  kor_enable_btn->set_position(kor_pos);
  kor_disable_btn->set_position(kor_pos);

  UICallbackFunctor eng_enable_functor(this, SR_UI_CALLBACK_SEL(LangButtonSelector::EngPressed));
  UICallbackFunctor eng_disable_functor(this, SR_UI_CALLBACK_SEL(LangButtonSelector::EngPressed));
  UICallbackFunctor kor_enable_functor(this, SR_UI_CALLBACK_SEL(LangButtonSelector::KorPressed));
  UICallbackFunctor kor_disable_functor(this, SR_UI_CALLBACK_SEL(LangButtonSelector::KorPressed));
  eng_enable_btn->set_pressed_functor(eng_enable_functor);
  eng_disable_btn->set_pressed_functor(eng_disable_functor);
  kor_enable_btn->set_pressed_functor(kor_enable_functor);
  kor_disable_btn->set_pressed_functor(kor_disable_functor);

  //국가에 맞춰서 언어도 설정
  if (Locale::GetInstance().language() == kLanguageKorean) {
    KorPressed(NULL);
  } else {
    EngPressed(NULL);
  }
}
}