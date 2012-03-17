﻿/*  Copyright (C) 2011-2012 by if1live */
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

#include "nodepng/lodepng.h"

namespace sora {;

//format별 image loader
class PNGLoader {
public:
  PNGLoader();
  ~PNGLoader();

  bool Decode(uchar *data, int data_size);
  const void *data() const { return &image_[0]; }

  int width() const { return width_; }
  int height() const { return height_; }
  int bit_depth() const { return bit_depth_; }
  int bpp() const { return bpp_; }
  int color_channels() const { return color_channels_; }

  void PrintLog();

private:
  std::vector<unsigned char> image_;

  //image data
  int width_;
  int height_;
  int bit_depth_;
  int bpp_;
  int color_channels_;

  LodePNG::Decoder decoder_;
};

PNGLoader::PNGLoader()
: width_(0),
height_(0),
bit_depth_(0),
bpp_(0),
color_channels_(0) {
}

PNGLoader::~PNGLoader() {
}

bool PNGLoader::Decode(uchar *data, int data_size) {
  SR_ASSERT(data != NULL);
  SR_ASSERT(data_size > 0);
  image_.clear();
  
  decoder_.decode(image_, data, data_size);
  if(decoder_.hasError()) { 
    return false;

  } else {
    width_ = decoder_.getWidth();
    height_ = decoder_.getHeight();
    bit_depth_ = decoder_.getInfoPng().color.bitDepth;
    bpp_ = decoder_.getBpp();
    color_channels_ = decoder_.getChannels();

    return true;
  }
}
void PNGLoader::PrintLog() {
  if(decoder_.hasError()) {
    std::cout << "error " << decoder_.getError() << ": " << LodePNG_error_text(decoder_.getError()) << std::endl;
  } else {
    std::cout << "\n" <<
      "w: " << decoder_.getWidth() << "\n" <<
      "h: " << decoder_.getHeight() << "\n" <<
      "bitDepth: " << decoder_.getInfoPng().color.bitDepth << "\n" <<
      "bpp: " << decoder_.getBpp() << "\n" <<
      "colorChannels: " << decoder_.getChannels() << "\n" <<
      "paletteSize: " << decoder_.getInfoPng().color.palettesize / 4 << "\n" <<
      "colorType: " << decoder_.getInfoPng().color.colorType << "\n" <<
      "compressionMethod: " << decoder_.getInfoPng().compressionMethod << "\n" <<
      "filterMethod: " << decoder_.getInfoPng().filterMethod << "\n" <<
      "interlaceMethod: " << decoder_.getInfoPng().interlaceMethod << "\n";
    for(size_t i = 0; i < decoder_.getInfoPng().text.num; i++) {
      std::cout << decoder_.getInfoPng().text.keys[i] << ": " << decoder_.getInfoPng().text.strings[i] << "\n";
    }
  }
}

////////////////////////////////////////////

Texture::Texture()
: start_(NULL), 
end_(NULL), 
handle_(0),
file_fmt_(kFileUnknown),
tex_width_(0),
tex_height_(0),
src_width_(0),
src_height_(0) {
}

Texture::~Texture() {
}

void Texture::Deinit() {
  if(handle_ != 0) {
    glDeleteTextures(1, &handle_);
    handle_ = 0;
  }
}
void Texture::SetData(int file, uchar *start, uchar *end) {
  file_fmt_ = file;
  start_ = start;
  end_ = end;
}

bool Texture::Init(GLuint tex_id, int width, int height) {
  SR_ASSERT(Loaded() == false);
  handle_ = tex_id;
  file_fmt_ = kFileUnknown;
  start_ = NULL;
  end_ = NULL;
  tex_width_ = width;
  tex_height_ = height;
  src_width_ = width;
  src_height_ = height;
}

bool Texture::Loaded() const {
  if(handle_ == 0) {
    return false;
  } else {
    return true;
  }
}
bool Texture::Init() {
  SR_ASSERT(Loaded() == false);
  bool result = false;

  switch(file_fmt_) {
  case kFileUnknown:
    result = false;
    break;

  case kFilePNG:
    result = Init_PNG();
    break;

  default:
    result = false;
    break;
  }
  //clear data
  file_fmt_ = kFileUnknown;
  start_ = NULL;
  end_ = NULL;
  return result;
}

bool Texture::Init_PNG() {
  int size = end_ - start_;
  PNGLoader loader;
  bool decode_result = loader.Decode(start_, size);
  if(decode_result == false) {
    return false;
  }
  //create opengl texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &handle_);
  glBindTexture(GL_TEXTURE_2D, handle_);

  GLenum format = GL_RGBA;
  if(loader.color_channels() == 4) {
    format = GL_RGBA;
  } else if(loader.color_channels() == 3) {
    format = GL_RGB;
  } else {
    SR_ASSERT(!"unknown color channel");
  }

  tex_width_ = loader.width();
  tex_height_ = loader.height();
  src_width_ = loader.width();
  src_height_ = loader.height();

  GLenum elem_type = GL_UNSIGNED_BYTE;
  if(loader.bit_depth() == 8) {
    elem_type = GL_UNSIGNED_BYTE;
  } else {
    SR_ASSERT(!"unknowkn bit depth");
  }

  glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width_, tex_height_, 0, format, elem_type, loader.data());
  
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return true;
}

}