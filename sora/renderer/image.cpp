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
#include "image.h"

#include "lodepng/lodepng.h"
#include "core/math_helper.h"
#include "soil/SOIL.h"

namespace sora {;

class LodePNGLoader {
public:
  //lodepng용 추가 함수
  static void PrintLog(LodePNG::Decoder &decoder) {
    if(decoder.hasError()) {
      std::cout << "error " << decoder.getError() << ": " << LodePNG_error_text(decoder.getError()) << std::endl;
    } else {
      std::cout << "\n" <<
        "w: " << decoder.getWidth() << "\n" <<
        "h: " << decoder.getHeight() << "\n" <<
        "bitDepth: " << decoder.getInfoPng().color.bitDepth << "\n" <<
        "bpp: " << decoder.getBpp() << "\n" <<
        "colorChannels: " << decoder.getChannels() << "\n" <<
        "paletteSize: " << decoder.getInfoPng().color.palettesize / 4 << "\n" <<
        "colorType: " << decoder.getInfoPng().color.colorType << "\n" <<
        "compressionMethod: " << decoder.getInfoPng().compressionMethod << "\n" <<
        "filterMethod: " << decoder.getInfoPng().filterMethod << "\n" <<
        "interlaceMethod: " << decoder.getInfoPng().interlaceMethod << "\n";
      for(size_t i = 0; i < decoder.getInfoPng().text.num; i++) {
        std::cout << decoder.getInfoPng().text.keys[i] << ": " << decoder.getInfoPng().text.strings[i] << "\n";
      }
    }
  }

  static bool Load(Image *img, uchar *data, int data_size) {
    //lodepng를 기반으로 적절히 로딩하기  
    SR_ASSERT(data != NULL);
    SR_ASSERT(data_size > 0);
    img->image_data_.clear();

    LodePNG::Decoder decoder;
    decoder.decode(img->image_data(), data, data_size);
    if(decoder.hasError()) { 
      return false;

    } else {
      img->width_ = decoder.getWidth();
      img->height_ = decoder.getHeight();
      img->bit_depth_ = decoder.getInfoPng().color.bitDepth;
      img->bpp_ = decoder.getBpp();
      img->color_channels_ = decoder.getChannels();
      img->is_grayscale_ = decoder.isGreyscaleType();
      img->is_alpha_ = decoder.isAlphaType();

      return true;
    }
  }
};

class SOILLoader {
public:
  static bool Load(Image *img, uchar *data, int data_size) {
    //귀찮은 관계로 soil에 떠넘기자
    //압축된거같은 데이터에서 쌩 데이터로 떠내기
    int width, height, channels;
    unsigned char *raw_img = SOIL_load_image_from_memory(
      data, data_size, &width, &height, &channels, SOIL_LOAD_AUTO
      );
    if(raw_img == nullptr) {
      return false;
    }

    img->image_data_.resize(width * height * channels * sizeof(uchar));
    memcpy(img->image_data_.data(), raw_img, img->image_data_.size() * sizeof(uchar));
    SOIL_free_image_data(raw_img);

    //텍스쳐에서 얻어낸 속성을 적절히 재가공
    img->width_ = width;
    img->height_ = height;
    img->bit_depth_ = 8; //?
    img->bpp_ = 8 * channels;
    img->color_channels_ = channels;

    switch(channels) {
    case SOIL_LOAD_L:
      img->is_grayscale_ = true;
      img->is_alpha_ = false;
      break;
    case SOIL_LOAD_LA:
      img->is_grayscale_ = true;
      img->is_alpha_ = true;
      break;
    case SOIL_LOAD_RGB:
      img->is_grayscale_ = false;
      img->is_alpha_ = false;
      break;
    case SOIL_LOAD_RGBA:
      img->is_grayscale_ = false;
      img->is_alpha_ = true;
      break;
    default:
      SR_ASSERT(!"do not reach");
      break;
    }
    return true;
  }
};

Image::Image()
  : width_(0),
height_(0),
bit_depth_(0),
bpp_(0),
color_channels_(0),
is_grayscale_(false),
is_alpha_(false) {

}

Image::~Image() {
}
bool Image::LoadPNG(uchar *data, int data_size) {
  //return LodePNGLoader::Load(this, data, data_size);
  return SOILLoader::Load(this, data, data_size);
}

bool Image::LoadJPG(uchar *data, int data_size) {
  return SOILLoader::Load(this, data, data_size);
}

bool Image::LoadETC(uchar *data, int data_size) {
  return SOILLoader::Load(this, data, data_size);
}

}
