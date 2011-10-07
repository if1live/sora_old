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
#include "precompile.h"
#if SR_WIN
#include "aki/image_loader.h"
#include "aki/image.h"

// 이미지 불러오는 기능을 윈도우에서는 SOIL로
// 하고 아이폰의 내장 라이브러리로 한다
#include <SOIL.h>

namespace aki {;
ImagePtr ImageLoader::Load(const std::string &path) {
  SR_ASSERT(path.length() > 0);
  int width, height, channels;
  unsigned char *data = SOIL_load_image(
    path.c_str(),
    &width,
    &height,
    &channels,
    SOIL_LOAD_AUTO);

  InternalFormat format;
  switch (channels) {
  case SOIL_LOAD_L:
    format = kInternalFormatLuminance;
    break;
  case SOIL_LOAD_LA:
    format = kInternalFormatLuminanceAlpha;
    break;
  case SOIL_LOAD_RGB:
    format = kInternalFormatRGB;
    break;
  case SOIL_LOAD_RGBA:
    format = kInternalFormatRGBA;
    break;
  }
  PixelType pixel_type = kPixelTypeByte;

  ImageDescription desc(width, height, format, pixel_type);
  return ImagePtr(new Image(desc, data));
}
}

#endif
