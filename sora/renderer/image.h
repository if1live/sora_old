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
#pragma once

namespace sora {;

class LodePNGLoader;
class SOILLoader;
//이미지를 적절히 로딩해서 보관하는 클래스이다
//이것을 기반으로 gl/gles/dx같은곳에서 로딩을 할수도 있도록 하자
//이미지 데이터 자체를 멤버변수로 저장할수 있으면
//추가적인 작업이 가능할거다
class Image {
public:
  friend class LodePNGLoader;
  friend class SOILLoader;
public:
  Image();
  ~Image();

public:
  bool LoadPNG(uchar *data, int data_size);
  bool LoadJPG(uchar *data, int data_size);
  //정체를 명시적으로 알수없는것에 대해서 적절히 읽기
  bool LoadETC(uchar *data, int data_size);

public:
  int width() const { return width_; }
  int height() const { return height_; }
  int bit_depth() const { return bit_depth_; }
  int bpp() const { return bpp_; }
  int color_channels() const { return color_channels_; }
  bool is_grayscale() const { return is_grayscale_; }
  bool is_alpha() const { return is_alpha_; }
  const uchar *data() const { return image_data_.data(); }
  uchar *data() { return image_data_.data(); }
  std::vector<uchar> &image_data() { return image_data_; }

private:
  int width_;
  int height_;
  int bit_depth_;
  int bpp_;
  int color_channels_;
  bool is_grayscale_;
  bool is_alpha_;

  std::vector<uchar> image_data_;
};
}
