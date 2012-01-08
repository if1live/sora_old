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
#include "intro_scene.h"

#include "sora/sprite_sheet_manager.h"

#include "sora/button.h"
#include "sora/image_label.h"
#include "sora/memory_file.h"
#include "sora/ui_container.h"
#include "sora/ui_drawer.h"
#include "sora/texture_manager.h"
#include "sora/texture.h"
#include "sora/selector.h"

#include "sora/locale.h"
#include "sora/gl_window.h"

#include "sora/immediate_mode_emulator.h"
#include "sora/matrix_stack.h"

#include "game_scene.h"
#include "menu_scene.h"
#include "sora/scene_manager.h"
#include "lang_button_selector.h"

#include "book.h"
#include "book_scene.h"

using namespace sora;

namespace yukino {;

struct IntroSceneButtonSelector : public Selector {
public:
  virtual void MenuPressed(UIComponent *obj) {
    MenuScene *menu_scene = new MenuScene();
    SceneManager::GetInstance().PopAndDestroy();
    SceneManager::GetInstance().Push(menu_scene);
  }
  virtual void StartPressed(UIComponent *obj) {
    GameScene *game_scene = new GameScene(0);
    SceneManager::GetInstance().PopAndDestroy();
    SceneManager::GetInstance().Push(game_scene);

    // load first page
    Book &book = Book::GetInstance();
    book.SetLanguage(Locale::GetInstance().language());
    BookScene *page = book.GetCurrScene();
    page->LoadTexture();
  }
};

struct IntroSceneImpl {
  sora::UIContainer ui_container;
  TextureHandle eng_background_tex_handle;
  TextureHandle kor_background_tex_handle;
  IntroSceneButtonSelector btn_selector;
  LangButtonSelector lang_selector;
};

IntroScene::IntroScene() 
: impl_(NULL) {

  impl_ = new IntroSceneImpl();

  //ui에서 쓰이는 스프라이트 로딩
  MemoryFile sprite_xml_file("ui/ui_sprite-hd.xml");
  sprite_xml_file.Open();
  TextureAtlas tex_atlas = SpriteSheetManager::Read((const char*)sprite_xml_file.start, "/ui/");
  SpriteSheetManager::GetInstance().Save(tex_atlas);
  {
    Texture *ui_tex = TextureManager::GetInstance().GetTexture(tex_atlas.tex_handle);
    TextureParameter param;
    param.mag_filter = kTexMagLinear;
    param.min_filter = kTexMinLinearMipMapNearest;
    param.wrap_s = kTexWrapRepeat;
    param.wrap_t = kTexWrapRepeat;
    ui_tex->SetTextureParameter(param);
  }
  TextureManager::GetInstance().AsyncLoad(tex_atlas.tex_handle);

  TextureSubImage *start_img = tex_atlas.GetSubImage("BtMenuStart@2x");
  TextureSubImage *menu_img = tex_atlas.GetSubImage("BtMenuPage@2x");
  
  
  SR_ASSERT(start_img != NULL);
  SR_ASSERT(menu_img != NULL);

  //float win_w = GLWindow::GetInstance().width();
  //float win_h = GLWindow::GetInstance().height();

  //왼쪽 아래에 대충 kor/eng
  {
    Button *eng_disable_btn = new Button();
    Button *eng_enable_btn = new Button();
    Button *kor_disable_btn = new Button();
    Button *kor_enable_btn = new Button();

    impl_->ui_container.Add(eng_disable_btn);
    impl_->ui_container.Add(eng_enable_btn);
    impl_->ui_container.Add(kor_disable_btn);
    impl_->ui_container.Add(kor_enable_btn);

    impl_->lang_selector = LangButtonSelector(eng_enable_btn, eng_disable_btn, kor_enable_btn, kor_disable_btn);
    impl_->lang_selector.Set(0, 0);
  }

  //가운데에 start
  {
    Button *start_btn = new Button(*start_img);
    vec2 pos(300, 300);
    start_btn->set_position(pos);
    impl_->ui_container.Add(start_btn);

    UICallbackFunctor start_functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(IntroSceneButtonSelector::StartPressed));
    start_btn->set_released_functor(start_functor);
  }

  //오른쪽 아래에 menu
  {
    Button *menu_btn = new Button(*menu_img);
    vec2 pos(400, 400);
    menu_btn->set_position(pos);
    impl_->ui_container.Add(menu_btn);

    UICallbackFunctor menu_functor(&impl_->btn_selector, SR_UI_CALLBACK_SEL(IntroSceneButtonSelector::MenuPressed));
    menu_btn->set_released_functor(menu_functor);
  }

  // 배경 이미지. 국자거에 따라서 띄우기. 이것도 쓰레드 로딩이 필요한가?
  Locale &locale = Locale::GetInstance();
  const char eng_menu[] = "background/MenuB_E@2x.png";
  const char kor_menu[] = "background/MenuB_K@2x.png";
  for (int i = 0 ; i < 2 ; i++) {
    const char *menu_file = NULL;
    TextureHandle *tex_handle = NULL;
    if (i == 0) {
      menu_file = kor_menu;
      tex_handle = &impl_->kor_background_tex_handle;
    } else {
      menu_file = eng_menu;
      tex_handle = &impl_->eng_background_tex_handle;
    }

    Texture *back_tex = TextureManager::GetInstance().CreateTexture(*tex_handle);
    back_tex->set_filename(menu_file);
    TextureParameter param;
    param.mag_filter = kTexMagLinear;
    param.min_filter = kTexMinLinearMipMapNearest;
    param.wrap_s = kTexWrapRepeat;
    param.wrap_t = kTexWrapRepeat;
    back_tex->SetTextureParameter(param);
    TextureManager::GetInstance().AsyncLoad(*tex_handle);
  }
}
IntroScene::~IntroScene() {
  //쌩 배경은 더이상 사용하지 않으니 그냥 내리자
  TextureManager::GetInstance().RemoveTexture(impl_->eng_background_tex_handle);
  TextureManager::GetInstance().RemoveTexture(impl_->kor_background_tex_handle);

  delete(impl_);
  impl_ = NULL;
}

void IntroScene::Draw() {
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //배경텍스쳐로 이미지 그리기
  Texture *tex = NULL;
  if (Locale::GetInstance().language() == kLanguageEnglish) {
    tex = TextureManager::GetInstance().GetTexture(impl_->eng_background_tex_handle);
  } else {
    tex = TextureManager::GetInstance().GetTexture(impl_->kor_background_tex_handle);
  }
  
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
void IntroScene::Update(int dt_ms) {
}
}
