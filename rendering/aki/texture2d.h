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
#ifndef RENDERING_AKI_TEXTURE2D_H_
#define RENDERING_AKI_TEXTURE2D_H_

#include "aki/aki_enum.h"
#include "sora/id_generator.h"

namespace aki {;
class TextureSize {
 public:
  TextureSize();
  TextureSize(int orig_width, int orig_height, int tex_width, int tex_height);
  ~TextureSize();
  void SetZero();

 public:
  int orig_width;
  int orig_height;
  int tex_width;
  int tex_height;
};

// opengl texture
template<unsigned int N>
class Texture2DGroup : public sora::IdGenerator<int>, boost::noncopyable {
 public:
  Texture2DGroup();
  ~Texture2DGroup();

  bool Initialize();
  bool Deinitialize();

  GLuint handle() const;
  GLuint handle(int index) const;
  template<int index> GLuint handle() const;

  void Bind() const;

  /// @brief 단일텍스쳐의 경우 외부라이브러리에 의해서 생성된 텍스쳐 정보를 바로 갖다쓰기위한거
  bool SetTexture(GLuint tex_id, const TextureSize &size);

  const TextureSize &size() const;
  const TextureSize &size(int index) const;
  template<int index>
  const TextureSize &size() const;

  bool IsLoaded() const;
  bool IsLoaded(int index) const;
  template<int index>
  bool IsLoaded() const;

 private:
  GLuint handle_[N];
  TextureSize tex_size_[N];
  int texture_count_;
};
}

#include "aki/texture2d_impl.h"

#endif  // RENDERING_AKI_TEXTURE2D_H_
