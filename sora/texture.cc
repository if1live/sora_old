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
#include "sora/texture.h"

namespace sora {;
GLenum Texture::ConvertToGLenum(TexMinFilter orig) {
  switch (orig) {
  case kTexMinLinearMipMapLinear:
    return GL_LINEAR_MIPMAP_LINEAR;
  case kTexMinLinearMipMapNearest:
    return GL_LINEAR_MIPMAP_NEAREST;
  case kTexMinLinearMipMapOff:
    return GL_LINEAR;
  case kTexMinNearestMipMapLinear:
    return GL_NEAREST_MIPMAP_LINEAR;
  case kTexMinNearestMipMapNearest:
    return GL_NEAREST_MIPMAP_NEAREST;
  case kTexMinNearestMipMapOff:
    return GL_NEAREST;
  default:
    SR_ASSERT(!"do not reach");
    return GL_NEAREST;
  }
}
GLenum Texture::ConvertToGLenum(TexMagFilter orig) {
  switch (orig) {
  case kTexMagLinear:
    return GL_LINEAR;
  case kTexMagNearest:
    return GL_NEAREST;
  default:
    SR_ASSERT(!"do not reach");
    return GL_NEAREST;
  }
}
GLenum Texture::ConvertToGLenum(TexWrapMode orig) {
  switch (orig) {
  case kTexWrapClamp:
    return GL_CLAMP_TO_EDGE;
  case kTexWrapRepeat:
    return GL_REPEAT;
  case kTexWrapMirroredRepeat:
    return GL_MIRRORED_REPEAT;
  default:
    SR_ASSERT(!"do not reach");
    return GL_REPEAT;
  }
}

Texture::Texture()
  : handle(0) {
  // 초기화 코드 공유를 위해서
  Cleanup();
}
Texture::~Texture() {
  Cleanup();
}
void Texture::Cleanup() {
  wrap_s = kTexWrapRepeat;
  wrap_t = kTexWrapRepeat;
  min_filter = kTexMinNearestMipMapOff;
  mag_filter = kTexMagNearest;
  mipmap = false;
  format = kTexFormatAuto;

  if (handle == 0) {
    return;
  }
  srglDeleteTextures(1, &handle);
}
Texture &Texture::Sample() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    unsigned char tex_data[] = {
      255, 0, 0, 255,
      0, 255, 0, 255,
      0, 0, 255, 255,
      255, 255, 255, 255,
    };
    InitSimpleTexture(2, 2, kTexFormatRGBA8888, tex_data, &tex);
  }
  return tex;
}
Texture &Texture::White() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    unsigned char tex_data[] = {
      255, 255, 255, 255,
      255, 255, 255, 255,
      255, 255, 255, 255,
      255, 255, 255, 255,
    };
    InitSimpleTexture(2, 2, kTexFormatRGBA8888, tex_data, &tex);
  }
  return tex;
}
Texture &Texture::Black() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    unsigned char tex_data[] = {
      0, 0, 0, 255,
      0, 0, 0, 255,
      0, 0, 0, 255,
      0, 0, 0, 255,
    };
    InitSimpleTexture(2, 2, kTexFormatRGBA8888, tex_data, &tex);
  }
  return tex;
}
Texture &Texture::Gray() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    unsigned char tex_data[] = {
      128, 128, 128, 255,
      128, 128, 128, 255,
      128, 128, 128, 255,
      128, 128, 128, 255,
    };
    InitSimpleTexture(2, 2, kTexFormatRGBA8888, tex_data, &tex);
  }
  return tex;
}
void Texture::InitSimpleTexture(i32 width, i32 height, TexFormat fmt,
  void *data, Texture *tex) {
  
  if (fmt == kTexFormatRGBA8888) {
    GLuint tex_id;
    srglGenTextures(1, &tex_id);
    srglBindTexture(GL_TEXTURE_2D, tex_id);
    srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    tex->format = fmt;
    tex->handle = tex_id;
    tex->mag_filter = kTexMagNearest;
    tex->min_filter = kTexMinNearestMipMapOff;
    tex->wrap_s = kTexWrapRepeat;
    tex->wrap_t = kTexWrapRepeat;
    tex->mipmap = false;

    tex->tex_header.bpp = 32;
    tex->tex_header.src_width = 2;
    tex->tex_header.src_height = 2;
    tex->tex_header.tex_width = 2;
    tex->tex_header.tex_height = 2;

  } else {
    SR_ASSERT(!"not support yet");
  }
}
}
