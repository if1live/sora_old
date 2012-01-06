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
#ifndef SORA_TEXTURE_INFO_H_
#define SORA_TEXTURE_INFO_H_

#include "handle.h"

namespace sora {;
typedef enum {
  kTexWrapRepeat,
  kTexWrapClamp,
  kTexWrapMirroredRepeat,
  kTexWrapCount,
} TexWrapMode;

typedef enum {
  kTexMagNearest,
  kTexMagLinear,
  kTexMagCount,
} TexMagFilter;

typedef enum {
  kTexMinNearestMipMapOff,
	kTexMinNearestMipMapNearest,
	kTexMinNearestMipMapLinear,
	kTexMinLinearMipMapOff,
	kTexMinLinearMipMapNearest,
	kTexMinLinearMipMapLinear,
  kTexMinCount,
} TexMinFilter;

typedef enum {
  kTexFormatAuto = 0,
  kTexFormatRGBA8888,
  kTexFormatRGBA4444,
  kTexFormatRGBA5551,
  kTexFormatRGB888,
  kTexFormatRGB565,
  kTexFormatLuminance,
  kTexFormatAlpha,
  kTexFormatLuminanceAlpha,
  kTexFormatCount,
} TexFormat;

struct TextureHeader {
  TextureHeader()
    : src_width(0),
    src_height(0),
    tex_width(0),
    tex_height(0),
    bpp(0) {}
  i32 src_width;
  i32 src_height;
  i32 tex_width;
  i32 tex_height;
  i8 bpp;
};

class TextureParameter {
public:
  static GLenum ConvertToGLenum(TexMinFilter orig);
  static GLenum ConvertToGLenum(TexMagFilter orig);
  static GLenum ConvertToGLenum(TexWrapMode orig);
  static GLenum ConvertToGLenum(TexFormat orig);
  static TexMinFilter ConvertToTexMinFilter(GLenum orig);
  static TexMagFilter ConvertToTexMagFilter(GLenum orig);
  static TexWrapMode ConvertToTexWrapMode(GLenum orig);
  static boolean IsMipMap(TexMinFilter min_filter);

  static int SearchTable(int table[][2], int count, int in_index, int out_index, int target);

public:
  TexMinFilter min_filter;
  TexMagFilter mag_filter;
  TexWrapMode wrap_s;
  TexWrapMode wrap_t;

  GLenum gl_min_filter() const { return ConvertToGLenum(min_filter); }
  GLenum gl_mag_filter() const { return ConvertToGLenum(mag_filter); }
  GLenum gl_wrap_s() const { return ConvertToGLenum(wrap_s); }
  GLenum gl_wrap_t() const { return ConvertToGLenum(wrap_t); }
  boolean IsMipMap() const { return IsMipMap(min_filter); }
};

struct TextureTag { };
typedef Handle<TextureTag> TextureHandle;
}

#endif