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
#ifndef SORA_TEXTURE_ATLAS_H_
#define SORA_TEXTURE_ATLAS_H_

#include "texture_info.h"

namespace sora {;
class Texture;
class TextureAtlas;
struct TextureSubImage;

// 텍스쳐 아틀라스에서의 좌표계 원점은 왼쪽위이다
// 왜냐하면 일반적으로 이미지 좌표계를 사용할때는 왼쪽위를 기준으로 쓰니까
// 텍스쳐를 쓰레드로 로딩하면 텍스쳐 id가 중간에 바뀐다
// 그래서 텍스쳐는 썡gltexid가 아니라 포인터(또는 엔진용핸들)로 갖고있어야한다
struct TextureSubImage {
  TextureSubImage();
  TextureSubImage(f32 x, f32 y, f32 w, f32 h, const TextureHandle &tex);

  // 텍스쳐 이미지에서의 좌표(이미지 크기 기준)
  f32 x;
  f32 y;
  // 단축명과 풀네임을 동일하게 취급할수 있도록함
  union {
    f32 width;
    f32 w;
  };
  union {
    f32 height;
    f32 h;
  };

  // opengl texture
  TextureHandle tex_handle;

  // 텍스쳐 좌표기준(0~1로 보정된 값)
  f32 GetTexCoordX();
  f32 GetTexCoordY();
  f32 GetTexCoordWidth();
  f32 GetTexCoordHeight();
  f32 GetTexWidth();
  f32 GetTexHeight();
  GLuint GetHandle();
};

class TextureAtlas {
public:
  TextureAtlas();
  TextureAtlas(const TextureHandle &handle);
  ~TextureAtlas();

  const TextureSubImage *GetSubImage(const char *key) const;
  TextureSubImage *GetSubImage(const char *key);
  const TextureSubImage *GetSubImage(const std::string &key) const {
    return GetSubImage(key.c_str());
  }
  TextureSubImage *GetSubImage(const std::string &key) {
    return GetSubImage(key.c_str());
  }

  TextureSubImage &AddSubImage(const std::string &key, f32 x, f32 y, f32 w, f32 h);

public:
  TextureHandle tex_handle;

private:
  typedef std::tr1::unordered_map<std::string, TextureSubImage> SubImageDictType;
  SubImageDictType sub_img_list_;
};

}

#endif  // SORA_TEXTURE_ATLAS_H_