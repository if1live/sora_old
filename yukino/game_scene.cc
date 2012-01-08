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
#include "game_scene.h"

#include "sora/selector.h"

#include "sora/memory_file.h"
#include "sora/sprite_sheet_manager.h"
#include "sora/texture_atlas.h"
#include "sora/texture_manager.h"

#include "sora/ui_component.h"
#include "sora/ui_container.h"
#include "sora/ui_drawer.h"
#include "sora/button.h"
#include "sora/image_label.h"


#include "glassless3d.h"

using namespace sora;

namespace yukino {;

class UIEventTestClass : public sora::Selector {
public:
  virtual void OnButtonPressed(sora::UIComponent *btn) {
    printf("btn pressed\n");
  }
  virtual void OnButtonReleased(sora::UIComponent *btn) {
    printf("btn released\n");
  }
};

struct GameSceneImpl {
  sora::UIContainer ui_container;
};


GameScene::GameScene()
: impl_(NULL) {
  impl_ = new GameSceneImpl();

  yukino::Glassless3d::GetInstance().Init();

  //테스트용 UI만들기

  //ui에서 쓰이는 스프라이트 로딩
  MemoryFile sprite_xml_file("ui/ui_sprite-hd.xml");
  sprite_xml_file.Open();
  TextureAtlas tex_atlas = SpriteSheetManager::Read((const char*)sprite_xml_file.start, "/ui/");
  SpriteSheetManager::GetInstance().Save(tex_atlas);
  //메인 메뉴에서 ui관련내용은 이미 읽어져있을 것이다
  //TextureManager::GetInstance().AsyncLoad(tex_atlas.tex_handle);

  TextureSubImage *next_img = tex_atlas.GetSubImage("BtMainNext@2x");
  TextureSubImage *prev_img = tex_atlas.GetSubImage("BtMainPrev@2x");
  TextureSubImage *reset_img = tex_atlas.GetSubImage("BtMainReset@2x");
  TextureSubImage *menu_img = tex_atlas.GetSubImage("BtMainPage@2x");
  SR_ASSERT(next_img != NULL);
  SR_ASSERT(prev_img != NULL);
  SR_ASSERT(reset_img != NULL);
  SR_ASSERT(menu_img != NULL);

  {
    ImageLabel *label = new ImageLabel(*prev_img);
    label->set_position(vec2(100, 100));
    impl_->ui_container.Add(label);
  }
  {
    static UIEventTestClass obj;
    Button *btn = new Button(*next_img);
    btn->set_position(vec2(400, 100));

    UICallbackFunctor pressed_functor(&obj, SR_UI_CALLBACK_SEL(UIEventTestClass::OnButtonPressed));
    btn->set_pressed_functor(pressed_functor);
    UICallbackFunctor released_functor(&obj, SR_UI_CALLBACK_SEL(UIEventTestClass::OnButtonReleased));
    btn->set_released_functor(released_functor);

    btn->set_pressed_img(*prev_img);


    impl_->ui_container.Add(btn);
  }
  {
    ImageLabel *label = new ImageLabel(*reset_img);
    label->set_position(vec2(100, 400));
    impl_->ui_container.Add(label);
  }
  {
    ImageLabel *label = new ImageLabel(*menu_img);
    label->set_position(vec2(400, 400));
    impl_->ui_container.Add(label);
  }
}
GameScene::~GameScene() {
  delete(impl_);
  impl_ = NULL;
}
void GameScene::Draw() {
  // OpenGL rendering goes here...
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  yukino::Glassless3d::GetInstance().Draw();

  //테스트 UI그리기
  sora::UIDrawer drawer;
  drawer.DrawRoot(&impl_->ui_container);
  drawer.DrawTouchArea(&impl_->ui_container);
}
void GameScene::Update(int dt_ms) {
  yukino::Glassless3d::GetInstance().Update(dt_ms * 0.001f);
}
}