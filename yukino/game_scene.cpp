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

#if SR_USE_PCH == 0
#include <boost/foreach.hpp>
#include "sora/gl_inc.h"
#endif

using namespace sora;
using namespace std;

namespace yukino {;

class GameSceneButtonSelector : public Selector {
public:
  virtual void OnNext(UIComponent *btn) {
    Book &book = Book::GetInstance();
    if (book.IsNextSceneExist()) {
      int old_page = book.curr_scene_page();
      Book::GetInstance().MoveNextScene();
      int new_page = book.curr_scene_page();

      LoadUnloadTexture(old_page, new_page);
    }
  }
  virtual void OnPrev(UIComponent *btn) {
    Book &book = Book::GetInstance();
    if (book.IsPrevScenExist()) {
      int old_page = book.curr_scene_page();
      Book::GetInstance().MovePrevScene();
      int new_page = book.curr_scene_page();

      LoadUnloadTexture(old_page, new_page);
    }
  }
  virtual void OnReset(UIComponent *btn) {
    // 어떻게 리셋??
    //Glassless3d::GetInstance().res
  }
  virtual void OnMenu(UIComponent *btn) {
    MenuScene *scene = new MenuScene();
    SceneManager::GetInstance().PopAndDestroy();
    SceneManager::GetInstance().Push(scene);

    for (int i = 0 ; i < Book::GetInstance().SceneCount() ; i++) {
      BookScene *scene = Book::GetInstance().GetScene(i);
      scene->UnloadTexture();
    }
  }

  void LoadUnloadTexture(int old_index, int new_index) {
    Book &book = Book::GetInstance();
    //BookScene *old_page = book.GetScene(old_index);
    BookScene *new_page = book.GetScene(new_index);

    //const set<TextureHandle> &old_tex_list = old_page->tex_handle_list();
    const set<TextureHandle> &new_tex_list = new_page->tex_handle_list();

    //모든 텍스쳐를 내리라고 요청
    //load요청이 있는것은 set에서 제거
    set<TextureHandle> unload_set;
    for(int i = 0 ; i < book.SceneCount() ; i++) {
      BookScene *scene = book.GetScene(i);
      const set<TextureHandle> &tex_list = scene->tex_handle_list();
      BOOST_FOREACH(const TextureHandle &handle, tex_list) {
        unload_set.insert(handle);
      }
    }

    set<TextureHandle>::const_iterator it;
    set<TextureHandle>::const_iterator endit;
  
    // new에 존재하는 텍스쳐는 unload목록에서 제거
    it = new_tex_list.begin();
    endit = new_tex_list.end();
    for ( ; it != endit ; it++) {
      const TextureHandle &handle = *it;
      unload_set.erase(handle);
      TextureManager::GetInstance().AsyncLoad(handle);
    }

    //unload할거 unload
    it = unload_set.begin();
    endit = unload_set.end();
    for ( ; it != endit ; it++) {
      const TextureHandle &handle = *it;
      // unload한다는것은 마저 로딩할 필요가 없다는것.
      TextureManager::GetInstance().CancelAsyncLoad(handle);

      Texture *tex = TextureManager::GetInstance().GetTexture(handle);
      tex->Cleanup();
      tex->SetAsLoading();
    }
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