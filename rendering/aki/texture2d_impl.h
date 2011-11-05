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
#ifndef RENDERING_AKI_TEXTURE2D_IMPL_H_
#define RENDERING_AKI_TEXTURE_IMPL_H_
#include "aki/image.h"
#include "matsu/math_helper.h"
#include <boost/static_assert.hpp>

namespace aki {;
template<unsigned int N>
Texture2DGroup<N>::Texture2DGroup()
: texture_count_(0) {
  BOOST_STATIC_ASSERT(N >= 1);
  for(int i = 0 ; i < N ; i++) {
    handle_[i] = 0;
  }
}
template<unsigned int N>
Texture2DGroup<N>::~Texture2DGroup() {
  Deinitialize();
}

template<unsigned int N>
bool Texture2DGroup<N>::Initialize() {
  GLuint *ptr = reinterpret_cast<GLuint*>(&handle_);
  glGenTextures(N, ptr);
  texture_count_ = N;
  return true;
}
template<unsigned int N>
bool Texture2DGroup<N>::Deinitialize() {
  bool retval = false;
  if (texture_count_ > 0) {
    GLuint *ptr = reinterpret_cast<GLuint*>(&handle_);
    glDeleteTextures(texture_count_, ptr);
    retval = true;
  }
  for(int i = 0 ; i < N ; i++) {
    handle_[i] = 0;
  }
  texture_count_ = 0;
  return retval;
}
template<unsigned int N>
GLuint Texture2DGroup<N>::handle(int index) const {
  SR_ASSERT(index >= 0 && index < N);
  return handle_[index];
}

template<unsigned int N>
GLuint Texture2DGroup<N>::handle() const {
  return handle_[0];
}
template<unsigned int N>
template<int index>
GLuint Texture2DGroup<N>::handle() const {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return handle(index);
}

template<unsigned int N>
bool Texture2DGroup<N>::SetTexture(GLuint tex_id, const TextureSize &size) {
  // 기존에 생성된것이 있으면 파기
  Deinitialize();
  handle_[0] = tex_id;
  tex_size_[0] = size;
  texture_count_ = 0;
  return true;
}

template<unsigned int N>
const TextureSize &Texture2DGroup<N>::size() const {
  return tex_size_[0];
}

template<unsigned int N>
const TextureSize &Texture2DGroup<N>::size(int index) const {
  SR_ASSERT(index >= 0 && index < N);
  return tex_size_[index];
}

template<unsigned int N>
template<int index>
const TextureSize &Texture2DGroup<N>::size() const {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return size(index);
}

template<unsigned int N>
bool Texture2DGroup<N>::IsLoaded() const {
  return IsLoaded<0>();
}

template<unsigned int N>
bool Texture2DGroup<N>::IsLoaded(int index) const {
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
bool Texture2DGroup<N>::IsLoaded() const {
  BOOST_STATIC_ASSERT(index >= 0 && index < N);
  return IsLoaded(index);
}

template<unsigned int N>
void Texture2DGroup<N>::Bind() const {
  glBindTexture(GL_TEXTURE_2D, handle());
}

}
#endif  // RENDERING_AKI_TEXTURE2D_IMPL_H_
