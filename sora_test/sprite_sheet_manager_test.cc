/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the 'Software'), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "sora_test_stdafx.h"
#include "sora/sprite_sheet_manager.h"

#include "sora/texture_atlas.h"
#include "sora/texture.h"
#include "sora/texture_manager.h"

const char content[] = "\
<?xml version='1.0' encoding='UTF-8'?>\
<!-- Created with TexturePacker http://texturepacker.com-->\
<!-- $TexturePacker:SmartUpdate:a80c8934c3ce84cf1d1d48832083a80a$ -->\
<!--Format:\
n  => name of the sprite\
x  => sprite x pos in texture\
y  => sprite y pos in texture\
w  => sprite width (may be trimmed)\
h  => sprite height (may be trimmed)\
oX => sprite's x-corner offset (only available if trimmed)\
oY => sprite's y-corner offset (only available if trimmed)\
oW => sprite's original width (only available if trimmed)\
oH => sprite's original height (only available if trimmed)\
r => 'y' only set if sprite is rotated\
-->\
<TextureAtlas imagePath='test.png' width='512' height='1024'>\
  <sprite n='BtMainNext@2x' x='2' y='2' w='68' h='68'/>\
  <sprite n='BtMainPage@2x' x='72' y='2' w='68' h='68'/>\
  <sprite n='BtMainPrev@2x' x='142' y='2' w='68' h='68'/>\
  <sprite n='BtMainReset@2x' x='212' y='2' w='68' h='68'/>\
  <sprite n='BtMenuBgm@2x' x='282' y='2' w='90' h='90'/>\
  <sprite n='BtMenuBgmD@2x' x='374' y='2' w='90' h='90'/>\
  <sprite n='BtMenuEnglish@2x' x='2' y='94' w='103' h='90'/>\
  <sprite n='BtMenuEnglishD@2x' x='107' y='94' w='103' h='90'/>\
  <sprite n='BtMenuKorean@2x' x='212' y='94' w='103' h='90'/>\
  <sprite n='BtMenuKoreanD@2x' x='317' y='94' w='103' h='90'/>\
  <sprite n='BtMenuPage@2x' x='2' y='186' w='90' h='90'/>\
  <sprite n='BtMenuStart@2x' x='94' y='186' w='222' h='122'/>\
  <sprite n='PageB_S06@2x' x='2' y='310' w='200' h='133'/>\
  <sprite n='PageB_S07@2x' x='204' y='310' w='200' h='133'/>\
  <sprite n='PageB_S08@2x' x='2' y='445' w='200' h='133'/>\
  <sprite n='PageB_S09@2x' x='204' y='445' w='200' h='133'/>\
  <sprite n='PageB_S10@2x' x='2' y='580' w='200' h='133'/>\
</TextureAtlas>";

TEST(SpriteSheetManager, Load) {
  using namespace sora;

  const char res_path[] = "testdata/";
  TextureAtlas tex_atlas = SpriteSheetManager::Load(content, res_path);

  Texture *tex = TextureManager::GetInstance().GetTexture(tex_atlas.tex_handle);
  EXPECT_EQ(512, tex->tex_header.tex_width);
  EXPECT_EQ(1024, tex->tex_header.tex_height);

  TextureSubImage *subimg = NULL;
  //<sprite n='BtMainReset@2x' x='212' y='2' w='68' h='68'/>
  subimg = tex_atlas.GetSubImage("BtMainReset@2x");
  EXPECT_EQ(212, subimg->x);
  EXPECT_EQ(2, subimg->y);
  EXPECT_EQ(68, subimg->w);
  EXPECT_EQ(68, subimg->h);

  //<sprite n='BtMenuBgm@2x' x='282' y='2' w='90' h='90'/>
  subimg = tex_atlas.GetSubImage("BtMenuBgm@2x");
  EXPECT_EQ(282, subimg->x);
  EXPECT_EQ(2, subimg->y);
  EXPECT_EQ(90, subimg->w);
  EXPECT_EQ(90, subimg->h);

  subimg = tex_atlas.GetSubImage("fds90123r");
  EXPECT_EQ(NULL, subimg);
}

TEST(SpriteSheetManager, Save) {
  using namespace sora;

  const char res_path[] = "testdata/";
  TextureAtlas tex_atlas = SpriteSheetManager::Load(content, res_path);
  SpriteSheetManager::GetInstance().Save(tex_atlas);


  TextureSubImage *subimg = NULL;
  //<sprite n='BtMainReset@2x' x='212' y='2' w='68' h='68'/>
  subimg = SpriteSheetManager::GetInstance().GetSubImage("BtMainReset@2x");
  EXPECT_EQ(212, subimg->x);
  EXPECT_EQ(2, subimg->y);
  EXPECT_EQ(68, subimg->w);
  EXPECT_EQ(68, subimg->h);

  //<sprite n='BtMenuBgm@2x' x='282' y='2' w='90' h='90'/>
  subimg = SpriteSheetManager::GetInstance().GetSubImage("BtMenuBgm@2x");
  EXPECT_EQ(282, subimg->x);
  EXPECT_EQ(2, subimg->y);
  EXPECT_EQ(90, subimg->w);
  EXPECT_EQ(90, subimg->h);

  subimg = SpriteSheetManager::GetInstance().GetSubImage("fds90123r");
  EXPECT_EQ(NULL, subimg);
}
