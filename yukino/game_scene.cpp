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

#include "sora/io/memory_file.h"
#include "sora/render/sprite_sheet_manager.h"
#include "sora/render/texture_atlas.h"
#include "sora/render/texture_manager.h"
#include "sora/render/texture.h"

#include "sora/ui/ui_component.h"
#include "sora/ui/ui_container.h"
#include "sora/ui/ui_drawer.h"
#include "sora/ui/button.h"
#include "sora/ui/image_label.h"


#include "glassless3d.h"
#include "menu_scene.h"
#include "sora/render/scene_manager.h"

#include "book.h"
#include "logic_interface.h"

#if SR_USE_PCH == 0
#include <boost/foreach.hpp>
#include "sora/render/gl_inc.h"
#endif

using namespace sora;
using namespace std;

namespace yukino {;

class GameSceneButtonSelector : public Selector {
public:
  virtual void OnNext(UIComponent *btn) {
    sora_next_page();
  }
  virtual void OnPrev(UIComponent *btn) {
    sora_prev_page();
  }
  virtual void OnReset(UIComponent *btn) {
    // 어떻게 리셋??
    //Glassless3d::GetInstance().res
  }
  virtual void OnMenu(UIComponent *btn) {
    MenuScene *scene = new MenuScene();
    SceneManager::GetInstance().PopAndDestroy();
    SceneManager::GetInstance().Push(scene);

    sora_unload_texture();
  }
};

struct GameSceneImpl {
  GameSceneButtonSelector btn_selector;
  sora::UIContainer ui_container;
};

GameScene::GameScene(int page)
: impl_(NULL) {
  impl_ = new GameSceneImpl();

  yukino::Glassless3d::GetInstance().Init();  

  {
    //menu
    TextureSubImage *menu_img = SpriteSheetManager::GetInstance().GetSubImage("BtMainPage@2x");
    SR_ASSERT(menu_img != NULL);

    Button *btn = new Button(*menu_img);
    btn->set_position(vec2(100, 100));
    impl_->ui_container.Add(btn);

    UICallbackFunctor functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(GameSceneButtonSelector::OnMenu));
    btn->set_released_functor(functor);
  }

  {
    //reset
    TextureSubImage *reset_img = SpriteSheetManager::GetInstance().GetSubImage("BtMainReset@2x");
    SR_ASSERT(reset_img != NULL);

    Button *btn = new Button(*reset_img);
    btn->set_position(vec2(400, 100));
    impl_->ui_container.Add(btn);

    UICallbackFunctor functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(GameSceneButtonSelector::OnReset));
    btn->set_released_functor(functor);
  }

  {
    //prev
    TextureSubImage *prev_img = SpriteSheetManager::GetInstance().GetSubImage("BtMainPrev@2x");
    SR_ASSERT(prev_img != NULL);

    Button *btn = new Button(*prev_img);
    btn->set_position(vec2(100, 400));
    impl_->ui_container.Add(btn);

    UICallbackFunctor functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(GameSceneButtonSelector::OnPrev));
    btn->set_released_functor(functor);
  }

  {
    //next
    TextureSubImage *next_img = SpriteSheetManager::GetInstance().GetSubImage("BtMainNext@2x");
    SR_ASSERT(next_img != NULL);

    Button *btn = new Button(*next_img);
    btn->set_position(vec2(400, 400));
    impl_->ui_container.Add(btn);

    UICallbackFunctor functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(GameSceneButtonSelector::OnNext));
    btn->set_released_functor(functor);
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
  //sora::UIDrawer drawer(960, 640);
  sora::UIDrawer drawer(800, 480);
  drawer.DrawRoot(&impl_->ui_container);
  drawer.DrawTouchArea(&impl_->ui_container);
}
void GameScene::Update(int dt_ms) {
  yukino::Glassless3d::GetInstance().Update(dt_ms * 0.001f);
}
}