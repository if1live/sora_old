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
#include "aki/image.h"
#include <string>
#include "aki/image_description.h"
#if SR_WIN
#include "aki/image_loader_soil.h"
#elif
#error "not support"
#endif


namespace aki {;
ImagePtr Image::Create(const std::string &path) {
#if SR_WIN
  // use soil
  return ImageLoader_SOIL::Load(path);
#elif _IPHONE_
#error "not support"
  // use core graphic
#endif
}
Image::Image(const ImageDescription &desc, void *data)
  : desc_(desc), data_(data) {
}
Image::Image(int width, int height)
  : data_(NULL) {
  desc_ = ImageDescription(width, height, kInternalFormatRGBA, kPixelType8888);
  data_ = malloc(sizeof(unsigned char) * 4 * width * height);
  memset(data_, 0, sizeof(unsigned char) * 4 * width * height);
}
Image::~Image() {
  free(data_);
  data_ = NULL;
}
const ImageDescription &Image::desc() const {
  return desc_;
}
const void *Image::data() const {
  return data_;
}
void Image::Overwrite(const Image &o) {
  // 포맷이 동일할때만 덮어쓰기가 가능하게하자
  // 만약 포맷이 다를ㄹ떄 덮어쓰기를 지원할 경우는
  // 아직까지 코드상에서 등장할일이
  // 없을거같으니까 나중에 추가하자
  SR_ASSERT(desc_.internal_format() == o.desc().internal_format()
    && desc_.pixel_type() == o.desc().pixel_type());
  // 텍스쳐 크기 보정용인데 더 작은 이미지에 덮어쓰는 일은 없을것이다
  SR_ASSERT(desc_.width() >= o.desc().width()
    && desc_.height() >= o.desc().height());

  // 줄단위로 복사하기
  for (int y = 0 ; y < o.desc().height() ; y++) {
    int prev_index = o.GetPixelIndex(0, y);
    int curr_index = GetPixelIndex(0, y);
    unsigned char *prev_data = (unsigned char*)o.data();
    unsigned char *curr_data = (unsigned char*)data();

    unsigned char *src = prev_data + prev_index;
    unsigned char *dst = curr_data + curr_index;

    int rowline = o.desc().GetRowLineSize();
    memcpy(dst, src, rowline);
  }
}
int Image::GetPixelIndex(int x, int y) const {
  int width = desc_.width();
  int height = desc_.height();
  SR_ASSERT(x >= 0 && x < width && y >= 0 && y < height);
  int pixel_size = desc_.GetPixelSize();
  int linesize = desc_.GetRowLineSize();
  return (linesize * y) + (pixel_size * x);
}

bool Image::operator==(const Image &o) const {
  if (desc_ != o.desc_)   {
    return false;
  }
  int size = desc_.height() * desc_.GetRowLineSize();
  if (memcmp(data_, o.data_, size) == 0) {
    return true;
  }
  return false;
}
bool Image::operator!=(const Image &o) const {
  return !(*this == o);
}
}
