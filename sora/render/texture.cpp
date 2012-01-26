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
#include "sora_stdafx.h"

#if SR_WIN || SR_IOS
#include "sora/io/memory_file.h"
#else
#include "sora/io/zip_stream_file.h"
#endif

#include "texture_info.h"
#include "texture.h"
#include "gl_helper.h"
#include "texture_helper.h"

#include "sora/io/filesystem.h"

#if SR_USE_PCH == 0
#include <png.h>
#include "gl_inc.h"
#include <set>
#endif
#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace sora {;
Texture::Texture()
  : handle_(0) {
  // 최초에 텍스쳐가 할당되어있지 않으면 임시 텍스쳐를 대신 띄운다
  SetAsLoading();
}
Texture::~Texture() {
  Cleanup();
}
void Texture::Deinit() {
  Cleanup();
  SetAsLoading();
}

void Texture::SetAsSample() {
  Cleanup();  //기존에 뭔가 있으면 해제

  set_handle(TextureHelper::GetSampleTexture(&tex_header.tex_width, &tex_header.tex_height));
  tex_header.src_width = tex_header.tex_width;
  tex_header.src_height = tex_header.tex_height;
  format = kTexFormatRGB888;

  //텍스쳐 id만 교체하지 텍스쳐 정보는 날리지 않는다
  //이것까지 날리면 다시 로딩할떄 의도하지 않은 결과가 나온다
  //param_.wrap_s = kTexWrapRepeat;
  //param_.wrap_t = kTexWrapRepeat;
  //param_.min_filter = kTexMinNearestMipMapOff;
  //param_.mag_filter = kTexMagNearest;
}
void Texture::SetAsLoading() {
  Cleanup();  //기존에 뭔가 있으면 해제
  set_handle(TextureHelper::GetLoadingTexture(&tex_header.tex_width, &tex_header.tex_height));
  tex_header.src_width = tex_header.tex_width;
  tex_header.src_height = tex_header.tex_height;
  format = kTexFormatRGB888;
  //텍스쳐 id만 교체하지 텍스쳐 정보는 날리지 않는다
  //이것까지 날리면 다시 로딩할떄 의도하지 않은 결과가 나온다
  //param_.wrap_s = kTexWrapRepeat;
  //param_.wrap_t = kTexWrapRepeat;
  //param_.min_filter = kTexMinNearestMipMapOff;
  //param_.mag_filter = kTexMagNearest;
}
void Texture::Cleanup() {
  if (handle() == 0) {
    return;
  }

  //완전히 빈것으로 설정하기
  GLuint loading_tex_id = TextureHelper::GetLoadingTexture();
  GLuint sample_tex_id = TextureHelper::GetSampleTexture();
  if(loading_tex_id != handle() && sample_tex_id != handle()) {
    // sample텍스쳐는 공유하므로 삭제하지 않는다
    srglDeleteTextures(1, &handle_);
    set_handle(0);
  }
}

void Texture::InitSimpleTexture(i32 width, i32 height, const TexFormat &fmt, void *data) {
  Cleanup();

  static TextureParameter default_param;
  default_param.min_filter = kTexMinNearestMipMapOff;
  default_param.mag_filter = kTexMagNearest;
  default_param.wrap_s = kTexWrapRepeat;
  default_param.wrap_t = kTexWrapRepeat;

  SR_ASSERT(fmt == kTexFormatRGBA8888 && "not support other yet");
  TextureHeader header;
  header.bpp = 32;
  header.src_width = width;
  header.src_height = height;
  header.tex_width = width;
  header.tex_height = height;
  
  Init(fmt, header, default_param, data);
}


boolean Texture::LoadFromPNG(const char *filepath, Texture *tex) {
  LOGI("LoadFromPNG Start");
  TexFormat fmt;
  TextureHeader header;
  void *data = LoadPNG(filepath, &fmt, &header);

	//load complete, create texture
  TextureParameter param;
  param.mag_filter = kTexMagLinear;
  param.min_filter = kTexMinLinearMipMapNearest;
  param.wrap_s = kTexWrapRepeat;
  param.wrap_t = kTexWrapRepeat;
  tex->Init(fmt, header, param, data);

	//clean up
	//픽셀데이터는 나중에 따로 처리한다
	delete[]((unsigned char*)data);
	
	//////////////////////////////////

  LOGI("LoadFromPNG End");
	return true;
}
void* Texture::LoadPNG(const char *filepath, TexFormat *fmt, TextureHeader *tex_header) {
#if SR_WIN || SR_IOS
  MemoryFile asset(filepath);
  return LoadPNG(asset, fmt, tex_header);
#else
  ZipStreamFile asset(filepath);
  return LoadPNG(asset, fmt, tex_header);
#endif
}

void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length) {
	png_voidp a = png_get_io_ptr(png_ptr);
#if SR_WIN || SR_IOS
  MemoryFile *file = (MemoryFile*)a;
	file->Read((char*)data, length);
#else
  ZipStreamFile *file = (ZipStreamFile*)a;
	file->Read((char*)data, length);
#endif
}

void Texture::SetTextureParameter(const TextureParameter &param) {
  param_ = param;

  if (handle() != 0) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param_.gl_mag_filter());
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param_.gl_min_filter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param_.gl_wrap_s());
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param_.gl_wrap_t());
    //TODO use mipmap?
    if (param.IsMipMap()) {
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  }
}

void Texture::Init(const TexFormat &fmt, const TextureHeader &header, const TextureParameter &param, void *data) {
  Cleanup();

  this->format = fmt;
  this->tex_header = header;
  this->param_ = param;

  if (fmt == kTexFormatRGBA8888) {
    GLuint tex_id;
    srglGenTextures(1, &tex_id);
    srglBindTexture(GL_TEXTURE_2D, tex_id);
    srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param.gl_min_filter()); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param.gl_mag_filter()); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param.gl_wrap_s());
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param.gl_wrap_t());
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.tex_width, header.tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //use mipmap
    //GL_GENERATE_MIPMAP가 데탑gl에만 있다. gles는 다른 구현을 필요함
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, param.IsMipMap());
    if (param.IsMipMap()) {
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    this->set_handle(tex_id);

  } else {
    SR_ASSERT(!"not support yet");
  }
}

}

