﻿/*  Copyright (C) 2011 by if1live */
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
class Texture;

class Texture {
public:
  static GLuint GetSampleTexture(int *width = NULL, int *height = NULL);
  static GLuint GetLoadingTexture(int *width = NULL, int *height = NULL);
  static Texture &Sample();
  //테스트용 텍스쳐는 색깔별로
  static Texture &White();
  static Texture &Black();
  static Texture &Gray();
  static Texture &Red();
  static Texture &Blue();
  static Texture &Green();
  static Texture &LoadingTexture();
  static void ColorTexture(u8 r, u8 g, u8 b, Texture *tex);
  
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

  // loading, sample, color texture 같이 시스템을 위해서 만든 텍스쳐인가?
  // 자체적으로 구현한 텍스쳐인가?
  bool IsSystemTexture() const;

  // file class type에 따라서 적절히 열기
  template<typename T>
  static void* LoadPNG(T &asset, TexFormat *fmt, TextureHeader *tex_header);

private:
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

template<typename T>
void* Texture::LoadPNG(T &asset, TexFormat *fmt, TextureHeader *tex_header) {
  LOGI("Load PNG Texture Start : %s", asset.filepath().c_str());

  const int PngSigSize = 8;
	//png 로딩된 메모리
	png_bytep* row_ptrs = NULL;
	char *data = NULL;

	//http://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng/
	//http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams
	bool opened = asset.Open();
	SR_ASSERT(opened == true);
	const char *filename = asset.filepath().c_str();

	//header for testing if it is a png
	png_byte header[PngSigSize];
	//read the header
	asset.Read(header, PngSigSize);

	//test if png
	int is_png = png_sig_cmp(header, 0, PngSigSize);
	if (is_png != 0) {
		LOGE("Not a png file : %s", filename);
		return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
		NULL, NULL);
	if (!png_ptr) {
		LOGE("Unable to create png struct : %s", filename);
		return false;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		LOGE("Unable to create png info : %s", filename);
		return false;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		LOGE("Unable to create png end info : %s", filename);
		return false;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		LOGE("Error during setjmp : %s", filename);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		if(row_ptrs != NULL) {
			delete[](row_ptrs);
		}
		if(data != NULL) {
			delete[](data);
		}
		return false;
	}

	//init png reading
	//png_init_io(png_ptr, fp);
	png_set_read_fn(png_ptr, &asset, png_asset_read);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	png_uint_32 img_width = png_get_image_width(png_ptr, info_ptr);
	png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);

	//bits per CHANNEL! note: not per pixel!
	png_uint_32 bitdepth = png_get_bit_depth(png_ptr, info_ptr);
	//Number of channels
	png_uint_32 channels = png_get_channels(png_ptr, info_ptr);
	//Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
	png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

	if(color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}
	if(color_type == PNG_COLOR_TYPE_GRAY && bitdepth < 8) {
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}
	if(bitdepth == 16) {
		png_set_strip_16(png_ptr);
	} else {
		png_set_packing(png_ptr);
	}

	//텍스쳐 크기같은 정보 설정
  tex_header->src_width = img_width;
  tex_header->src_height = img_height;
  tex_header->tex_width = img_width;
  tex_header->tex_height = img_height;

  *fmt = kTexFormatRGBA8888;
	switch(color_type) {
	case PNG_COLOR_TYPE_PALETTE:
		//위에서 팔레트는 RGB변환했으니
    *fmt = kTexFormatRGB888;
    tex_header->bpp = 24;
		break;
	case PNG_COLOR_TYPE_GRAY:
    *fmt = kTexFormatLuminance;
    tex_header->bpp = 8;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
    *fmt = kTexFormatLuminanceAlpha;
    tex_header->bpp = 16;
		break;
	case PNG_COLOR_TYPE_RGB:
		*fmt = kTexFormatRGB888;
    tex_header->bpp = 24;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
    *fmt = kTexFormatRGBA8888;
    tex_header->bpp = 32;
		break;
	default: 
    SR_ASSERT(!"do not reach");
	}

	//Here's one of the pointers we've defined in the error handler section:
	//Array of row pointers. One for every row.
	row_ptrs = new png_bytep[img_height];

	//Alocate a buffer with enough space.
	//(Don't use the stack, these blocks get big easilly)
	//This pointer was also defined in the error handling section, so we can clean it up on error.
	data = new char[img_width * img_height * channels];
	//This is the length in bytes, of one row.
  const unsigned int stride = img_width * channels;

	//A little for-loop here to set all the row pointers to the starting
	//Adresses for every row in the buffer

	for (size_t i = 0; i < img_height; i++) {
		//Set the pointer to the data pointer + i times the row stride.
		//Notice that the row order is reversed with q.
		//This is how at least OpenGL expects it,
		//and how many other image loaders present the data.
		//unsigned int q = (img_height- i - 1) * stride;
		unsigned int q = (i) * stride;
		row_ptrs[i] = (png_bytep)data + q;
	}

	//And here it is! The actuall reading of the image!
	//Read the imagedata and write it to the adresses pointed to
	//by rowptrs (in other words: our image databuffer)
	png_read_image(png_ptr, row_ptrs);
	png_read_end(png_ptr, NULL);

  /*
	//색의속성중 알파값이 0이면 색 자체를 255,255,255,0로 돌리기
  //외곽선이 뭉개질때 사용
	if(format == kTexFormatRGB888) {
		for(size_t i = 0 ; i < img_width * img_height ; i++) {
			unsigned char *pixel = (unsigned char*)data + (i * format);
			if(*(pixel+3) == 0) {
				*(pixel+0) = 255;
				*(pixel+1) = 255;
				*(pixel+2) = 255;
			}
		}
	}
  */

  // clean png data
  delete[]((png_bytep)row_ptrs);
  //And don't forget to clean up the read and info structs !
	png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)0);

  return data;
}
}

#endif  // SORA_TEXTURE_H_