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
#include "texture.h"
#include "core/template_lib.h"

using namespace std;

namespace sora {;

ImageDesc Texture::CreateImageDesc(int w, int h, TexFormatType format) {
  ImageDesc desc;
  desc.width = w;
  desc.height = h;
  desc.bit_depth = 32;

  switch(format) {
  case kTexFormatAlpha:
    desc.bpp = 8;
    desc.color_channels = 1;
    desc.is_grayscale = false;
    desc.is_alpha = true;
    break;
  case kTexFormatLumiance:
    desc.bpp = 8;
    desc.color_channels = 1;
    desc.is_grayscale = true;
    desc.is_alpha = false;
    break;
  case kTexFormatLumianceAlpha:
    desc.bpp = 16;
    desc.color_channels = 2;
    desc.is_grayscale = true;
    desc.is_alpha = true;
    break;
  case kTexFormatRGB:
    desc.bpp = 24;
    desc.color_channels = 3;
    desc.is_grayscale = false;
    desc.is_alpha = false;
    break;
  case kTexFormatRGBA:
    desc.bpp = 32;
    desc.color_channels = 4;
    desc.is_grayscale = false;
    desc.is_alpha = true;
    break;
  default:
    SR_ASSERT(!"not valid tex format");
  }

  return desc;
}

/////////////////////////////////

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

}