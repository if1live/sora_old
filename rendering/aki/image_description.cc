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
#include "aki/image_description.h"

namespace aki {;
ImageDescription::ImageDescription()
  : width_(0),
  height_(0),
  internal_format_(kInternalFormatRGBA),
  pixel_type_(kPixelType8888) {
}
ImageDescription::ImageDescription(int width,
  int height, InternalFormat internal_format, PixelType pixel_type)
  : width_(width),
  height_(height),
  internal_format_(internal_format),
  pixel_type_(pixel_type) {
}
ImageDescription::~ImageDescription() {
}
int ImageDescription::width() const {
  return width_;
}
int ImageDescription::height() const {
  return height_;
}

InternalFormat ImageDescription::internal_format() const {
  return internal_format_;
}
PixelType ImageDescription::pixel_type() const {
  return pixel_type_;
}

int ImageDescription::GetPixelSize() const {
  int pixel_size = 0;
  switch (pixel_type()) {
  case kPixelType8888:
    pixel_size = 4;   // 4byte
    break;
  case kPixelType5551:
    pixel_size = 2;
    break;
  case kPixelType4444:
    pixel_size = 2;
    break;
  case kPixelType565:
    pixel_size = 2;
    break;
  default:
    SR_ASSERT(!"not valid");
  }
  return pixel_size;
}
int ImageDescription::GetRowLineSize() const {
  int pixel_size = GetPixelSize();
  int linesize = pixel_size * width_;
  return linesize;
}
bool ImageDescription::operator==(const ImageDescription &o) const {
  return (width_ == o.width_
    && height_ == o.height_
    && internal_format_ == o.internal_format_
    && pixel_type_ == o.pixel_type_);
}
bool ImageDescription::operator!=(const ImageDescription &o) const {
  return !(*this == o);
}
}
