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
#ifndef RENDERING_AKI_TEXTURE_H_
#define RENDERING_AKI_TEXTURE_H_

#include "aki/aki_enum.h"

namespace aki {;
class TextureSize {
 public:
  TextureSize();
  TextureSize(int orig_width, int orig_height, int tex_width, int tex_height);
  ~TextureSize();

  SR_GETTER(int, orig_width);
  SR_GETTER(int, orig_height);
  SR_GETTER(int, tex_width);
  SR_GETTER(int, tex_height);
};

// opengl texture
template<unsigned int N>
class TextureGroup {
 public:
  TextureGroup();
  ~TextureGroup();

  int id() const;

  GLuint handle() const;
  GLuint handle(int index) const;
  template<int index>
  GLuint handle() const;

  void Bind() const;

  bool LoadImage(Image *img);
  bool LoadImage(int index, Image *img);
  template<int index>
  bool LoadImage(Image *img);

  const TextureSize &size() const;
  const TextureSize &size(int index) const;
  template<int index>
  const TextureSize &size() const;

  bool IsLoaded() const;
  bool IsLoaded(int index) const;
  template<int index>
  bool IsLoaded() const;

 private:
  int id_;
  GLuint handle_[N];
  TextureSize tex_size_[N];

  static int next_id_;
};
}

#include "aki/texture_impl.h"

#endif  // RENDERING_AKI_TEXTURE_H_
