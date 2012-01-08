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
#include "sprite_sheet_manager.h"

#include "xml_node.h"
#include "xml_reader.h"
#include "texture_atlas.h"
#include "common_string.h"
#include "texture.h"
#include "texture_info.h"
#include "texture_manager.h"

#if SR_USE_PCH == 0
#include <boost/foreach.hpp>
#endif

namespace sora {;
SpriteSheetManager::SpriteSheetManager() {
}
SpriteSheetManager::~SpriteSheetManager() {
}
TextureAtlas SpriteSheetManager::Read(const char *content, const char *res_path) {
  using namespace std;

  XmlNode root;
  XmlReader xml_reader;
  bool read_result = xml_reader.Read(&root, content);
  SR_ASSERT(read_result == true);

  SR_ASSERT(root.name() == "TextureAtlas");
  const string &image_path = root.GetAttribute("imagePath");
  const string &width_str = root.GetAttribute("width");
  const string &height_str = root.GetAttribute("height");
  SR_ASSERT(!image_path.empty());
  SR_ASSERT(!width_str.empty());
  SR_ASSERT(!height_str.empty());
  int tex_width = StringToInt(width_str);
  int tex_height = StringToInt(height_str);
  SR_ASSERT(tex_width > 0);
  SR_ASSERT(tex_height > 0);
  
  // 해당 이름에 해당하는 텍스쳐를 얻거나 만들기
  string real_image_path = res_path;
  real_image_path += '/';
  real_image_path += image_path;

  Texture *tex = NULL;
  TextureHandle tex_handle = TextureManager::GetInstance().FileNameToHandle(real_image_path);
  if (tex_handle.IsNull()) {
    tex = TextureManager::GetInstance().CreateTexture(tex_handle);
    //printf("Texture filename 2: %s\n", real_image_path.c_str());
    tex->set_filename(real_image_path);
    TextureManager::GetInstance().RegisterFilename(real_image_path, tex_handle);

  } else {
    tex_handle = TextureManager::GetInstance().FileNameToHandle(real_image_path);
    tex = TextureManager::GetInstance().GetTexture(tex_handle);
  }
  tex->tex_header.src_width = tex_width;
  tex->tex_header.src_height = tex_height;
  tex->tex_header.tex_width = tex_width;
  tex->tex_header.tex_height = tex_height;

  TextureAtlas tex_atlas(tex_handle);

  XmlNodeListConstIterator it = root.ChildBegin();
  XmlNodeListConstIterator endit = root.ChildEnd();
  for ( ; it != endit ; it++) {
    XmlNode *node = *it;
    // n  => name of the sprite
    // x  => sprite x pos in texture
    // y  => sprite y pos in texture
    // w  => sprite width (may be trimmed)
    // h  => sprite height (may be trimmed)
    // oX => sprite's x-corner offset (only available if trimmed)
    // oY => sprite's y-corner offset (only available if trimmed)
    // oW => sprite's original width (only available if trimmed)
    // oH => sprite's original height (only available if trimmed)
    // r => 'y' only set if sprite is rotated
    //<sprite n="BtMainNext@2x" x="2" y="2" w="68" h="68"/>
    SR_ASSERT(node->name() == "sprite");
    //x, y, w, h만 일단 지원
    int x = StringToInt(node->GetAttribute("x"));
    int y = StringToInt(node->GetAttribute("y"));
    int w = StringToInt(node->GetAttribute("w"));
    int h = StringToInt(node->GetAttribute("h"));
    const string &name = node->GetAttribute("n");

    tex_atlas.AddSubImage(name, x, y, w, h);
  }
  return tex_atlas;
}
void SpriteSheetManager::Save(const TextureAtlas &atlas, const char *atlas_name) {
  atlas_list_.push_back(atlas);
}
TextureSubImage *SpriteSheetManager::GetSubImage(const char *key) {
  BOOST_FOREACH(TextureAtlas &atlas, atlas_list_) {
    TextureSubImage *img = atlas.GetSubImage(key);
    if (key != NULL) {
      return img;
    }
  }
  return NULL;
}
}