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

class Texture {
public:
  static GLuint GetSampleTexture(int *width, int *height);
  static Texture &Sample();
  //테스트용 텍스쳐는 색깔별로
  static Texture &White();
  static Texture &Black();
  static Texture &Gray();
  static Texture &Red();
  static Texture &Blue();
  static Texture &Green();
  static void ColorTexture(u32 r, u32 g, u32 b, Texture *tex);
  
public:
  static boolean LoadFromPNG(const std::string &filepath, Texture *tex) {
    return LoadFromPNG(filepath.c_str(), tex);
  }
  static boolean LoadFromPNG(const char *filepath, Texture *tex);

  // png파일을 불러서 픽셀 데이터로 반환. 동적할당된것이 던져지니 알아서 적절히 처리
  static void* LoadPNG(const char *filepath, TexFormat *fmt, TextureHeader *header);

private:
  void InitSimpleTexture(i32 width, i32 height, const TexFormat &fmt, void *data);
  void Init(const TexFormat &fmt, const TextureHeader &tex_header, const TextureParameter &param, void *data);

public:
  Texture();
  ~Texture();

  void Cleanup();
  void SetTextureParameter(const TextureParameter &param);
 
public:
  TexWrapMode wrap_s() const { return param_.wrap_s; }
  TexWrapMode wrap_t() const { return param_.wrap_t; }
  TexMinFilter min_filter() const { return param_.min_filter; }
  TexMagFilter mag_filter() const { return param_.mag_filter; }
  boolean mipmap() const { return param_.IsMipMap(); }
  
  TexFormat format;
  TextureHeader tex_header;

  GLuint handle;  // 텍스쳐 여러개를 그룹화로 쓰는건 아마도 당분간 필요없겠지
private:
  TextureParameter param_;
};
}

#endif  // SORA_TEXTURE_H_