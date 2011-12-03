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

namespace sora {;

typedef enum {
  kTexWrapRepeat,
  kTexWrapClamp,
  kTexWrapMirroredRepeat,
} TexWrapMode;

typedef enum {
  kTexMagNearest,
  kTexMagLinear,
} TexMagFilter;

typedef enum {
  kTexMinNearestMipMapOff,
	kTexMinNearestMipMapNearest,
	kTexMinNearestMipMapLinear,
	kTexMinLinearMipMapOff,
	kTexMinLinearMipMapNearest,
	kTexMinLinearMipMapLinear,
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

class Texture {
public:
  static Texture &Sample();
  static Texture &White();
  static Texture &Black();
  static Texture &Gray();

  static GLenum ConvertToGLenum(TexMinFilter orig);
  static GLenum ConvertToGLenum(TexMagFilter orig);
  static GLenum ConvertToGLenum(TexWrapMode orig);
  
  static void LoadFromPNG(const std::string &filepath, Texture *tex) {
    return LoadFromPNG(filepath.c_str(), tex);
  }
  static void LoadFromPNG(const char *filepath, Texture *tex);

private:
  static void InitSimpleTexture(i32 width, i32 height, TexFormat fmt,
    void *data, Texture *tex);

public:
  Texture();
  ~Texture();

  void Cleanup();

public:
  TexWrapMode wrap_s;
  TexWrapMode wrap_t;
  TexMinFilter min_filter;
  TexMagFilter mag_filter;
  boolean mipmap;
  TexFormat format;
  TextureHeader tex_header;

  GLuint handle;  // 텍스쳐 여러개를 그룹화로 쓰는건 아마도 당분간 필요없겠지
};
}

#endif  // SORA_TEXTURE_H_