// Ŭnicode please
#include "yukino_stdafx.h"
#include "logic_interface.h"

#include "book.h"
#include "sora/render/texture_manager.h"
#include "sora/render/texture.h"
#include "book_scene.h"
#include "sora/sys_locale.h"
#include "sora/render/immediate_mode_emulator.h"
#include "sora/render/texture_helper.h"

#include "sora/input/accelerometer.h"
#include "sora/input/gyro.h"
#include "input_handler.h"

#if SR_USE_PCH == 0
#include "sora/io/zip_stream_file.h"
#include <boost/foreach.hpp>
#endif

using namespace yukino;
using namespace sora;
using namespace std;

void LoadUnloadTexture(int old_index, int new_index) {
  Book &book = Book::GetInstance();
  //BookScene *old_page = book.GetScene(old_index);
  BookScene *new_page = book.GetScene(new_index);

  //const set<TextureHandle> &old_tex_list = old_page->tex_handle_list();
  const set<TexturePtr> &new_tex_list = new_page->tex_handle_list();

  //모든 텍스쳐를 내리라고 요청
  //load요청이 있는것은 set에서 제거
  set<TexturePtr> unload_set;
  for(int i = 0 ; i < book.SceneCount() ; i++) {
    BookScene *scene = book.GetScene(i);
    const set<TexturePtr> &tex_list = scene->tex_handle_list();
    BOOST_FOREACH(const TexturePtr &handle, tex_list) {
      unload_set.insert(handle);
    }
  }

  set<TexturePtr>::const_iterator it;
  set<TexturePtr>::const_iterator endit;

  // new에 존재하는 텍스쳐는 unload목록에서 제거
  it = new_tex_list.begin();
  endit = new_tex_list.end();
  for ( ; it != endit ; it++) {
    const TexturePtr &tex = *it;
    unload_set.erase(tex);
    TextureManager::GetInstance().AsyncLoad(tex);
    LOGI("Load texture %s", tex->filename().c_str());
  }

  //unload할거 unload
  it = unload_set.begin();
  endit = unload_set.end();
  for ( ; it != endit ; it++) {
    const TexturePtr &tex = *it;
    // unload한다는것은 마저 로딩할 필요가 없다는것.
    TextureManager::GetInstance().CancelAsyncLoad(tex);
    tex->Cleanup();
    tex->SetAsLoading();
    LOGI("Unload texture %s", tex->filename().c_str());
  }
}

void sora_next_page() {
  Book &book = Book::GetInstance();
  if (book.IsNextSceneExist()) {
    int old_page = book.curr_scene_page();
    Book::GetInstance().MoveNextScene();
    int new_page = book.curr_scene_page();

    LoadUnloadTexture(old_page, new_page);
  }

  sora_reset_gyro();
}
void sora_prev_page() {
  Book &book = Book::GetInstance();
  if (book.IsPrevScenExist()) {
    int old_page = book.curr_scene_page();
    Book::GetInstance().MovePrevScene();
    int new_page = book.curr_scene_page();

    LoadUnloadTexture(old_page, new_page);
  }

  sora_reset_gyro();
}
void sora_reset_gyro() {
  InputHandler *handler = InputHandler::GetLast();
  if(handler != NULL) {
    handler->Reset();
  }
}
void sora_reset_accel() {
  InputHandler *handler = InputHandler::GetLast();
  if(handler != NULL) {
    handler->Reset();
  }
}
void sora_update_gyro(float yaw, float roll, float pitch) {
  Gyro::GetInstance().Add(yaw, roll, pitch);
}
void sora_update_accel(float x, float y, float z) {
  Accelerometer::GetInstance().Add(x, y, z);
}

void sora_unload_texture() {
  for (int i = 0 ; i < Book::GetInstance().SceneCount() ; i++) {
    BookScene *scene = Book::GetInstance().GetScene(i);
    if(scene != NULL) {
      scene->UnloadTexture();
    }
  }
  TextureHelper::Unload();
}
void sora_reload_texture() {
  Book &book = Book::GetInstance();
  BookScene *page = book.GetCurrScene();
  if(page != NULL) {
    page->LoadTexture();
  } else {
    LOGE("empty");
  }
  TextureHelper::Load();
}

void sora_unload_shader() {
  ImmediateModeEmulator::GetInstance().Cleanup();
}

void sora_set_lang_korean() {
  Locale::GetInstance().SetKorea();
  Book &book = Book::GetInstance();
  book.SetLanguage(Locale::GetInstance().language());

  sora_reset_gyro();
}

void sora_set_lang_english() {
  Locale::GetInstance().SetUSA();
  Book &book = Book::GetInstance();
  book.SetLanguage(Locale::GetInstance().language());

  sora_reset_gyro();
}

bool sora_is_next_page_exist() {
  Book &book = Book::GetInstance();
  return book.IsNextSceneExist();
}
bool sora_is_prev_page_exist() {
  Book &book = Book::GetInstance();
  return book.IsPrevScenExist();
}
void sora_set_apk_file_path(const char *abs_path) {
#if SR_ANDROID
  ZipStreamFile::SetApkFile(abs_path);
#endif
}