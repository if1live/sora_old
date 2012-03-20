/*  Copyright (C) 2011-2012 by if1live */
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
#include "texture_manager.h"
#include "template_lib.h"
#include "texture.h"

using namespace std;

namespace sora {;

TextureManager& TextureManager::GetInstance() {
  static TextureManager ctx;
  return ctx;
}

TextureManager::TextureManager() {
}
TextureManager::~TextureManager() {
}

bool TextureManager::Add(const Texture &tex) {
  const string &name = tex.name();
  if(IsExist(name) == true) {
    //already exist
    return false;
  }

  TexturePtr cpy_tex(new Texture(tex));
  cpy_tex->Init();
  tex_dict_[name] = cpy_tex;
  
  return true;
}

bool TextureManager::IsExist(const std::string &name) const {
  auto found = tex_dict_.find(name);
  if(found == tex_dict_.end()) {
    return false;
  } else {
    return true;
  }
}

TexturePtr TextureManager::Get(const std::string &name) {
  auto found = tex_dict_.find(name);
  if(found == tex_dict_.end()) {
    static TexturePtr empty;
    return empty;
  } else {
    return found->second;
  }
}

TexturePtr TextureMgr_get(const std::string &name) {
  TextureManager &mgr = TextureManager::GetInstance();
  return mgr.Get(name);
}
Texture *TextureMgr_get_ptr(const std::string &name) {
  TextureManager &mgr = TextureManager::GetInstance();
  return mgr.Get_ptr(name);
}
}