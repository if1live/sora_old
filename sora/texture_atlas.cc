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
#include "texture_atlas.h"

#include "texture.h"
#include "texture_manager.h"

namespace sora {;
TextureSubImage::TextureSubImage()
  : x(0), y(0), w(0), h(0) {
}
TextureSubImage::TextureSubImage(f32 x, f32 y, f32 w, f32 h, const TextureHandle &tex)
  : x(x), y(y), w(w), h(h), tex_handle(tex) {
}
GLuint TextureSubImage::GetHandle() {
  Texture *tex = TextureManager::GetInstance().GetTexture(tex_handle);
  SR_ASSERT(tex != NULL);
  return tex->handle;
}
f32 TextureSubImage::GetTexWidth() {
  Texture *tex = TextureManager::GetInstance().GetTexture(tex_handle);
  SR_ASSERT(tex != NULL);
  float tex_width = static_cast<f32>(tex->tex_header.tex_width);
  return tex_width;
}
f32 TextureSubImage::GetTexHeight() {
  Texture *tex = TextureManager::GetInstance().GetTexture(tex_handle);
  SR_ASSERT(tex != NULL);
  float tex_height = static_cast<f32>(tex->tex_header.tex_height);
  return tex_height;
}
f32 TextureSubImage::GetTexCoordX() {
  Texture *tex = TextureManager::GetInstance().GetTexture(tex_handle);
  float tex_width = GetTexWidth();
  return x / tex_width;
}
f32 TextureSubImage::GetTexCoordY() {
  float tex_height = GetTexHeight();
  return y / tex_height;
}
f32 TextureSubImage::GetTexCoordWidth() {
  float tex_width = GetTexWidth();
  return width / tex_width;
}
f32 TextureSubImage::GetTexCoordHeight() {
  float tex_height = GetTexHeight();
  return height / tex_height;
}
////////////////////////////////
TextureAtlas::TextureAtlas() {}
TextureAtlas::TextureAtlas(const TextureHandle &handle) : tex_handle(handle) {}
TextureAtlas::~TextureAtlas() {}

const TextureSubImage *TextureAtlas::GetSubImage(const char *key) const {
  SubImageDictType::const_iterator it = sub_img_list_.begin();
  SubImageDictType::const_iterator endit = sub_img_list_.end();
  for ( ; it != endit ; it++) {
    if (it->first == key) {
      return &(it->second);
    }
  }
  return NULL;
}
TextureSubImage *TextureAtlas::GetSubImage(const char *key) {
  SubImageDictType::iterator it = sub_img_list_.begin();
  SubImageDictType::iterator endit = sub_img_list_.end();
  for ( ; it != endit ; it++) {
    if (it->first == key) {
      return &(it->second);
    }
  }
  return NULL;
}
TextureSubImage &TextureAtlas::AddSubImage(const std::string &key, f32 x, f32 y, f32 w, f32 h) {
  TextureSubImage img(x, y, w, h, tex_handle);
  sub_img_list_[key] = img;
  return sub_img_list_[key];
}
}