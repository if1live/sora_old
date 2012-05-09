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
#include "template_lib.h"
#include "gl_texture.h"
#include "image.h"

using namespace std;

namespace sora {;

Texture::Texture(unsigned int policy)
  : is_render_to_texture_(false), 
  tex_policy_(policy),
  handle_(0) {

}

Texture::Texture(const std::string &name, unsigned int policy)
  : name_(name), 
  is_render_to_texture_(false), 
  tex_policy_(policy),
  handle_(0) {
}

Texture::~Texture() {
}


//외부에서 생성된 GL텍스쳐를 sora텍스쳐로 사용하기
void Texture::Deinit() { 
  Policy::Deinit(&handle_); 
}
void Texture::Init() {
  Policy::Init(&handle_); 
}
//외부에서 생성된 텍스쳐를 직접 찔러넣기. renter texture나 외부 라이브러리에서 생성된 텍스쳐에서 쓴다
bool Texture::Init(HandleType handle, const ImageDesc &img_desc, bool is_rtt) {
  if(handle_ != 0) {
    Deinit();
  }
  handle_ = handle;
  img_desc_ = img_desc;
  is_render_to_texture_ = is_rtt;
  return true;
}

bool Texture::Loaded() const {
  return Policy::Loaded(handle_); 
}

bool Texture::is_alpha() const {
  return img_desc_.is_alpha; 
}
bool Texture::is_grayscale() const { 
  return img_desc_.is_grayscale; 
}
bool Texture::is_render_to_texture() const { 
  return is_render_to_texture_; 
}

bool Texture::LoadTexture(const Image &img) {
  img_desc_ = img.desc();
  return Policy::LoadTexture(handle_, img, tex_policy_);
}

//압축 풀린 데이터를 올리는 경우
bool Texture::LoadTexture(unsigned char *image, int w, int h, TexFormatType format, const TextureParam &param) {
  img_desc_ = CreateImageDesc(w, h, format);
  bool result = Policy::LoadTexture(handle_, image, img_desc_, tex_policy_);
  Policy::ApplyTextureParam(handle_, param);
  return result;
}

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