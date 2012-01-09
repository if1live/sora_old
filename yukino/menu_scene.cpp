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
#include "menu_scene.h"

#include "sora/scene_manager.h"
#include "sora/selector.h"

#include "sora/texture.h"
#include "sora/texture_manager.h"
#include "sora/immediate_mode_emulator.h"
#include "sora/texture_atlas.h"

#include "sora/ui_container.h"
#include "sora/ui_drawer.h"
#include "sora/button.h"

#include "sora/sprite_sheet_manager.h"
#include "lang_button_selector.h"
#include "game_scene.h"
#include "sora/locale.h"

#include "book.h"
#include "book_scene.h"

#if SR_USE_PCH == 0
#include "sora/gl_inc.h"
#endif

using namespace std;
using namespace sora;

// ui좌표계는 기존에 만들어진것을 그냥 그대로 갖다가쓰기 위해서서 이렇게처리함
// 늘어나더라도 큰 문제는 일단은 없을 것이다
const float win_width = 960.0f;
const float win_height = 640.0f;

namespace yukino {;

struct MenuSceneButtonSelector : public sora::Selector {
public:
  virtual void ScenePressed(UIComponent *obj) {
    Button *btn = static_cast<Button*>(obj);
    int scene_index = (int)btn->userdata();    

    SceneManager::GetInstance().PopAndDestroy();
    GameScene *scene = new GameScene(scene_index);
    SceneManager::GetInstance().Push(scene);

    // load first page
    Book &book = Book::GetInstance();
    book.SetLanguage(Locale::GetInstance().language());
    book.MoveScene(scene_index);
    BookScene *page = book.GetCurrScene();
    page->LoadTexture();
  }
  virtual void ClosePressed(UIComponent *btn) {
    Book &book = Book::GetInstance();
    int scene_index = book.curr_scene_page();

    book.SetLanguage(Locale::GetInstance().language());

    //언어가 바뀌엇을수도 있는데 확인이 귀찮으니 통째로 다시 부르기
    //Book::GetInstance().Reload();

    SceneManager::GetInstance().PopAndDestroy();
    GameScene *scene = new GameScene(scene_index);
    SceneManager::GetInstance().Push(scene);

    // load first page
    BookScene *page = book.GetCurrScene();
    page->LoadTexture();
  }
};

struct MenuSceneImpl {
  sora::UIContainer ui_container;
  TextureHandle background_tex_handle;
  MenuSceneButtonSelector btn_selector;
  LangButtonSelector lang_selector;

  Button *kor_close_btn;
  Button *eng_close_btn;
};

MenuScene::MenuScene()
: impl_(NULL) {
  impl_ = new MenuSceneImpl();

  {
    // 배경 이미지. 이것도 쓰레드 로딩이 필요한가?
    const char back_file[] = "background/PageB@2x.png";

    Texture *back_tex = TextureManager::GetInstance().CreateTexture(impl_->background_tex_handle);
    back_tex->set_filename(back_file);
    TextureParameter param;
    param.mag_filter = kTexMagLinear;
    param.min_filter = kTexMinLinearMipMapNearest;
    param.wrap_s = kTexWrapRepeat;
    param.wrap_t = kTexWrapRepeat;
    back_tex->SetTextureParameter(param);
    TextureManager::GetInstance().AsyncLoad(impl_->background_tex_handle);
  }

  {
    float x_coord[] = { 25, 221, 413, 604 };
    float y_coord[] = { 24, 148, 270 };
    float btn_width = 180;
    float btn_height = 113;
    //960*640
    /*
    float x_coord[] = { 36, 264, 490, 726 };
    float y_coord[] = { 34, 196, 358 };
    float btn_width = 209;
    float btn_height = 141;
    */

    float btn_coord[][2] = {
      { x_coord[0], y_coord[0] },
      { x_coord[1], y_coord[0] },
      { x_coord[2], y_coord[0] },
      { x_coord[3], y_coord[0] },
      { x_coord[0], y_coord[1] },
      { x_coord[1], y_coord[1] },
      { x_coord[2], y_coord[1] },
      { x_coord[3], y_coord[1] },
      { x_coord[0], y_coord[2] },
      { x_coord[1], y_coord[2] },
    };
    for (int i = 0 ; i < 10 ; i++) {
      vec2 btn_pos(btn_coord[i][0], btn_coord[i][1]);
      //버튼
      Button *btn = new Button();
      btn->set_position(btn_pos);
      btn->set_touch_rect(Recti(vec2(0, 0) + btn_pos, vec2(btn_width, btn_height)));
      btn->set_userdata((void*)(i * 2));  //2배인 이유는 장면/대사 순서니까
      impl_->ui_container.Add(btn);

      UICallbackFunctor functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(MenuSceneButtonSelector::ScenePressed));
      btn->set_released_functor(functor);
    }
  }

  
  {
    // 언어 교체 버튼
    Button *eng_disable_btn = new Button();
    Button *eng_enable_btn = new Button();
    Button *kor_disable_btn = new Button();
    Button *kor_enable_btn = new Button();

    impl_->ui_container.Add(eng_disable_btn);
    impl_->ui_container.Add(eng_enable_btn);
    impl_->ui_container.Add(kor_disable_btn);
    impl_->ui_container.Add(kor_enable_btn);

    impl_->lang_selector = LangButtonSelector(eng_enable_btn, eng_disable_btn, kor_enable_btn, kor_disable_btn);
    impl_->lang_selector.Set(25, 390);
  }
  {
    // 뒤로가기 버튼
    const char kor_file[] = "BtCloseKorean@2x";
    const char eng_file[] = "BtCloseEnglish@2x";
    TextureSubImage *eng_img = SpriteSheetManager::GetInstance().GetSubImage(eng_file);
    TextureSubImage *kor_img = SpriteSheetManager::GetInstance().GetSubImage(kor_file);
    Button *eng_btn = new Button(*eng_img);
    Button *kor_btn = new Button(*kor_img);

    impl_->ui_container.Add(eng_btn);
    impl_->ui_container.Add(kor_btn);
    impl_->eng_close_btn = eng_btn;
    impl_->kor_close_btn = kor_btn;

    UICallbackFunctor functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(MenuSceneButtonSelector::ClosePressed));
    eng_btn->set_released_functor(functor);
    kor_btn->set_released_functor(functor);

    vec2 pos(400, 400);
    eng_btn->set_position(pos);
    kor_btn->set_position(pos);
  }
}
MenuScene::~MenuScene() {
  if (impl_ != NULL) {
    delete(impl_);
    impl_ = NULL;
  }
}

void MenuScene::Draw() {
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //배경텍스쳐로 이미지 그리기
  Texture *tex = TextureManager::GetInstance().GetTexture(impl_->background_tex_handle);
  if (tex != NULL) {
    int back_width = tex->tex_header.src_width;
    int back_height = tex->tex_header.src_height;
  
    srglMatrixMode(SR_PROJECTION);
    srglLoadIdentity();
    srglOrtho(0, 1, 0, 1, -1, 1);
  
    srglMatrixMode(SR_MODELVIEW);
    srglLoadIdentity();

    srglBindTexture(GL_TEXTURE_2D, tex->handle());
    srglBegin(GL_QUADS);
    srglTexCoord2f(0, 1); srglVertex2f(0, 0);
    srglTexCoord2f(1, 1); srglVertex2f(1, 0);
    srglTexCoord2f(1, 0); srglVertex2f(1, 1);
    srglTexCoord2f(0, 0); srglVertex2f(0, 1);
    srglEnd();
  }

  //sora::UIDrawer drawer(960, 640);
  sora::UIDrawer drawer(800, 480);
  drawer.DrawRoot(&impl_->ui_container);
  drawer.DrawTouchArea(&impl_->ui_container); 
}
void MenuScene::Update(int dt_ms) {
  if (Locale::GetInstance().language() == kLanguageEnglish) {
    impl_->eng_close_btn->set_visible(true);
    impl_->kor_close_btn->set_visible(false);
  } else {
    impl_->eng_close_btn->set_visible(false);
    impl_->kor_close_btn->set_visible(true);
  }
}
}