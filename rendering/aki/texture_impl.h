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
#ifndef RENDERING_AKI_TEXTURE_IMPL_H_
#define RENDERING_AKI_TEXTURE_IMPL_H_
#include "aki/image.h"
#include "matsu/math.h"


namespace aki {;
template<unsigned int N>
int TextureGroup<N>::next_id_ = 0;

template<unsigned int N>
TextureGroup<N>::TextureGroup()
  : id_(next_id_++) {
  BOOST_STATIC_ASSERT(N >= 1);
  GLuint *ptr = reinterpret_cast<GLuint*>(&handle_);
  glGenTextures(N, ptr);
}
template<unsigned int N>
TextureGroup<N>::~TextureGroup() {
  GLuint *ptr = reinterpret_cast<GLuint*>(&handle_);
  glDeleteTextures(N, ptr);
}

template<unsigned int N>
int TextureGroup<N>::id() const {
  return id_;
}
template<unsigned int N>
GLuint TextureGroup<N>::handle(int index) const {
  SR_ASSERT(index >= 0 && index < N);
  return handle_[index];
}

template<unsigned int N>
GLuint TextureGroup<N>::handle() const {
  return handle_[0];
}
template<unsigned int N>
template<int index>
GLuint TextureGroup<N>::handle() const {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return handle(index);
}
template<unsigned int N>
bool TextureGroup<N>::LoadImage(Image *img) {
  return LoadImage(0, img);
}

template<unsigned int N>
bool TextureGroup<N>::LoadImage(int index, Image *img) {
  SR_ASSERT(index >= 0 && index < N);
  const ImageDescription &desc = img->desc();
  int width = desc.width();
  int height = desc.height();
  int tex_width = matsu::CeilPower(2, width);
  int tex_height = matsu::CeilPower(2, height);

  TextureSize size(width, height, tex_width, tex_height);
  tex_size_[index] = size;

  // bind+set tex param
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, handle_[index]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image
  // 텍스쳐 크기 보정때문에 여기부터는 약간 달라진다
  if (width != tex_width || height != tex_height) {
    // 2의 승수로 보정한 이미지 생성
    Image newImg(tex_width, tex_height);
    newImg.Overwrite(*img);

    const void *data = newImg.data();
    SR_ASSERT(data != NULL);

    int internal_format = newImg.desc().internal_format();
    int pixel_type = newImg.desc().pixel_type();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, tex_width, tex_height,
      0, internal_format, pixel_type, data);
  } else {
    const void *data = img->data();
    SR_ASSERT(data != NULL);

    int internal_format = img->desc().internal_format();
    int pixel_type = img->desc().pixel_type();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, tex_width, tex_height,
      0, internal_format, pixel_type, data);
  }

  return true;
}

template<unsigned int N>
template<int index>
bool TextureGroup<N>::LoadImage(Image *img) {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return LoadImage(index, img);
}

template<unsigned int N>
const TextureSize &TextureGroup<N>::size() const {
  return tex_size_[0];
}

template<unsigned int N>
const TextureSize &TextureGroup<N>::size(int index) const {
  SR_ASSERT(index >= 0 && index < N);
  return tex_size_[index];
}

template<unsigned int N>
template<int index>
const TextureSize &TextureGroup<N>::size() const {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return size(index);
}

template<unsigned int N>
bool TextureGroup<N>::IsLoaded() const {
  return IsLoaded<0>();
}

template<unsigned int N>
bool TextureGroup<N>::IsLoaded(int index) const {
  SR_ASSERT(index >= 0 && index < N);
  const TextureSize &size = size();
  if (size.getTexWidth() == 0 && size.getTexHeight() == 0) {
    return false;
  } else {
    return true;
  }
}
template<unsigned int N>
template<int index>
bool TextureGroup<N>::IsLoaded() const {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return IsLoaded(index);
}
}
#endif  // RENDERING_AKI_TEXTURE_IMPL_H_
