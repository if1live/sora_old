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
#include "main_scene.h"

#include "sora/selector.h"

#include "sora/io/memory_file.h"
#include "sora/render/sprite_sheet_manager.h"
#include "sora/render/texture_atlas.h"
#include "sora/render/texture_manager.h"
#include "sora/render/texture.h"

#include "glassless3d.h"
#include "menu_scene.h"
#include "sora/render/scene_manager.h"

#include "book.h"
#include "logic_interface.h"
#include "sora/render/gl_helper.h"

#if SR_USE_PCH == 0
#include <boost/foreach.hpp>
#include "sora/render/gl_inc.h"
#endif

using namespace sora;
using namespace std;

namespace yukino {;

MainScene::MainScene() {
  //적당한 언어로 일단 열기
  Book &book = Book::GetInstance();
  book.SetLanguage(Locale::GetInstance().language());
  BookScene *page = book.GetCurrScene();
  page->LoadTexture();

  yukino::Glassless3d::GetInstance().Init();
}
MainScene::~MainScene() {
}
void MainScene::Draw() {
  // OpenGL rendering goes here...
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  yukino::Glassless3d::GetInstance().Draw();
}
void MainScene::Update(int dt_ms) {
  yukino::Glassless3d::GetInstance().Update(dt_ms * 0.001f);
}
}