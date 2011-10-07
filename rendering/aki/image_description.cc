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
  : width(0),
  height(0),
  internal_format(kInternalFormatRGBA),
  pixel_type(kPixelTypeByte) {
}
ImageDescription::ImageDescription(int width,
  int height, InternalFormat internal_format, PixelType pixel_type)
  : width(width),
  height(height),
  internal_format(internal_format),
  pixel_type(pixel_type) {
}
ImageDescription::~ImageDescription() {
}
int ImageDescription::GetRowLineSize() const {
  int pixel_size = GetPixelSize();
  int linesize = pixel_size * width;
  return linesize;
}
bool ImageDescription::operator==(const ImageDescription &o) const {
  return (width == o.width
    && height == o.height
    && internal_format == o.internal_format
    && pixel_type == o.pixel_type);
}
bool ImageDescription::operator!=(const ImageDescription &o) const {
  return !(*this == o);
}
int ImageDescription::GetPixelSize() const {
  int pixel_size = 0;
  if (internal_format == kInternalFormatAlpha) {
    if (pixel_type == kPixelTypeByte) {
      pixel_size = 1;
    }

  } else if (internal_format == kInternalFormatLuminance) {
    if (pixel_type == kPixelTypeByte) {
      pixel_size = 1;
    }

  } else if (internal_format == kInternalFormatLuminanceAlpha) {
    if (pixel_type == kPixelTypeByte) {
      pixel_size = 2;
    }

  } else if (internal_format == kInternalFormatRGB) {
    if (pixel_type == kPixelType565) {
      pixel_size = 2;
    } else if (pixel_type == kPixelTypeByte) {
      pixel_size = 3;
    }

  } else if (internal_format == kInternalFormatRGBA) {
    if (pixel_type == kPixelType4444) {
      pixel_size = 2;
    } else if (pixel_type == kPixelType5551) {
      pixel_size = 2;
    } else if (pixel_type == kPixelTypeByte) {
      pixel_size = 4;
    }
  }
  SR_ASSERT(pixel_size > 0);
  return pixel_size;
}
}
