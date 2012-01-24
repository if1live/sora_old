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
#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "texture_info.h"

#if SR_USE_PCH == 0
#include <png.h>
#endif

namespace sora {;
class Texture {  
public:
  static boolean LoadFromPNG(const std::string &filepath, Texture *tex) {
    return LoadFromPNG(filepath.c_str(), tex);
  }
  static boolean LoadFromPNG(const char *filepath, Texture *tex);

  // png파일을 불러서 픽셀 데이터로 반환. 동적할당된것이 던져지니 알아서 적절히 처리
  static void* LoadPNG(const char *filepath, TexFormat *fmt, TextureHeader *header);
  void Init(const TexFormat &fmt, const TextureHeader &tex_header, const TextureParameter &param, void *data);
  // 현재 올려져있는 텍스쳐를 내리고 sample로 교체하기
  void Deinit();

  void SetAsSample();
  void SetAsLoading();

  // file class type에 따라서 적절히 열기
  template<typename T>
  static void* LoadPNG(T &asset, TexFormat *fmt, TextureHeader *tex_header);

  void InitSimpleTexture(i32 width, i32 height, const TexFormat &fmt, void *data);

public:
  Texture();
  ~Texture();

  // 텍스쳐만 내리기. 텍스쳐는 비어있다
  void Cleanup();
  void SetTextureParameter(const TextureParameter &param);
  const TextureParameter &param() const { return param_; }
 
public:
  TexWrapMode wrap_s() const { return param_.wrap_s; }
  TexWrapMode wrap_t() const { return param_.wrap_t; }
  TexMinFilter min_filter() const { return param_.min_filter; }
  TexMagFilter mag_filter() const { return param_.mag_filter; }
  boolean mipmap() const { return param_.IsMipMap(); }
  
  TexFormat format;
  TextureHeader tex_header;

  GLuint handle() const { return handle_; }
  void set_handle(GLuint tex_id) { handle_ = tex_id; }

  // 파일명을 적절히 저장해놓으면 내려놨던 텍스쳐 다시 불러오는게 가능하겠지?
  const std::string filename() const { return filename_; }
  void set_filename(const char *file) { filename_ = file; }
  void set_filename(const std::string &file) { filename_ = file; }

private:
  TextureParameter param_;
  std::string filename_;  // 내렸던 텍스쳐를 다시 올리거나 할때 사용
  GLuint handle_;  // 텍스쳐 여러개를 그룹화로 쓰는건 아마도 당분간 필요없겠지
};


void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length);

}

#include "texture.hpp"

#endif  // SORA_TEXTURE_H_