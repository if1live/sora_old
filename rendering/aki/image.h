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
#ifndef RENDERING_AKI_IMAGE_H_
#define RENDERING_AKI_IMAGE_H_

#include <string>
#include "aki/aki_enum.h"
#include "aki/image_description.h"

namespace aki {;
class Image {
 public:
  static ImagePtr Create(const std::string &path);

 public:
  // RGBA를 사용한 검정색 이미지 만들기
  // 이미지가 텍스쳐 생성하기위한 크기가 아닐때 사용한다
  Image(int width, int height);
  Image(const ImageDescription &desc, void *data);
  ~Image();
  const ImageDescription &desc() const;
  const void *data() const;

  // 다른 이미지를 덮어씌우는 함수. 이것이 있어야
  // 텍스쳐크기를 2의 승수로 보정하는것을 깨끗하게 할수있다
  void Overwrite(const Image &o);

  int GetPixelIndex(int x, int y) const;

  bool operator==(const Image &o) const;
  bool operator!=(const Image &o) const;

 private:
  ImageDescription desc_;
  void *data_;
};
}
#endif  // RENDERING_AKI_IMAGE_H_
