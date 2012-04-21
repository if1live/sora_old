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
#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "renderer/gl/gl_texture.h"
#include "core/shared_ptr_inc.h"
#include "core/unordered_map_inc.h"

namespace sora {;

template<typename T> class TextureT;
typedef TextureT<sora::gl::GLTexture> Texture;
typedef std::tr1::shared_ptr<Texture> TexturePtr;

template<typename T>
class TextureT {
public:
  TextureT(const std::string &name, unsigned int policy = 0)
  : obj_(name, policy) {}
  ~TextureT() {}

  //외부에서 생성된 GL텍스쳐를 sora텍스쳐로 사용하기
  //bool Init(uint tex_id, int width, int height, bool has_alpha, bool is_rtt);
  //bool Reload(GLTexture &data)

  void Deinit() { obj_.Deinit(); }
  void SetData(TexFileType file_fmt, uchar *start, uchar *end) {
    obj_.SetData(file_fmt, start, end);
  }
  void Init() { obj_.Init(); }
  bool Loaded() const { return obj_.Loaded(); }
  const std::string &name() const { return obj_.name(); }
  bool has_alpha() const { return obj_.has_alpha(); }
  bool is_render_to_texture() const { return obj_.is_render_to_texture(); }

  bool LoadTexture(unsigned char *image, int w, int h, TexFormatType format, const TextureParam &param) {
    return obj_.LoadTexture(image, w, h, format, param);
  }
  T &obj() { return obj_; }

private:
  T obj_;
};

//텍스쳐 관리까지 한군데에 전부 떠넘기자
class TextureManager {
public:
  TextureManager();
  ~TextureManager();

  //텍스쳐 파일정보와 메모리를 설정한 다음에 텍스쳐를 넣으면
  //복사해서 적절히 생성함
  bool Add(const Texture &tex);

  bool IsExist(const std::string &name) const;
  TexturePtr Get(const std::string &name);
  Texture *Get_ptr(const std::string &name) { return Get(name).get(); }

private:
  typedef std::tr1::unordered_map<std::string, TexturePtr> TexDictType;

  TexDictType tex_dict_;
};
}
#endif  // SORA_TEXTURE_H_