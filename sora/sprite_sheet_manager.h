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
#ifndef SORA_SPRITE_SHEET_MANAGER_H_
#define SORA_SPRITE_SHEET_MANAGER_H_

#include "template_library.h"

#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;
class TextureAtlas;
class TextureSubImage;

class SpriteSheetManager : public Singleton<SpriteSheetManager> {
public:
  typedef std::vector<TextureAtlas> AtlasListType;
public:
  SpriteSheetManager();
  ~SpriteSheetManager();

  static TextureAtlas Read(const char *content, const char *res_path);
  // 텍스쳐 아틀라스에는 이름을 아직 쓰지 않지만
  // 나중에 이름을 붙여서 관리해서 메모리에 올렸다 내렸다 할수있으니까 남겨놓자
  void Save(const TextureAtlas &atlas, const char *atlas_name = NULL);
  TextureSubImage *GetSubImage(const char *key);
private:
  AtlasListType atlas_list_;
};
}

#endif  // SORA_SPRITE_SHEET_MANAGER_H_