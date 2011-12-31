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
#include <png.h>
#include "sora/memory_file.h"

namespace sora {;

// 0 : sora enum
// 1 : gl enum
// 2 : use mipmap?
int tex_min_filter_table[][3] = {
  { kTexMinLinearMipMapLinear, GL_LINEAR_MIPMAP_LINEAR, 1 },
  { kTexMinLinearMipMapNearest, GL_LINEAR_MIPMAP_NEAREST, 1 },
  { kTexMinLinearMipMapOff, GL_LINEAR, 0 },
  { kTexMinNearestMipMapLinear, GL_NEAREST_MIPMAP_LINEAR, 1 },
  { kTexMinNearestMipMapNearest, GL_NEAREST_MIPMAP_NEAREST, 1 },
  { kTexMinNearestMipMapOff, GL_NEAREST, 0 },
};
int tex_mag_filter_table[][2] = {
  { kTexMagLinear, GL_LINEAR },
  { kTexMagNearest, GL_NEAREST },
};
int tex_wrap_mode_table[][2] = {
  { kTexWrapClamp, GL_CLAMP_TO_EDGE },
  { kTexWrapRepeat, GL_REPEAT }, 
  { kTexWrapMirroredRepeat, GL_MIRRORED_REPEAT },
};
int tex_format_table[][2] = {
  { kTexFormatAuto, GL_RGBA },
  { kTexFormatRGBA8888, GL_RGBA },
  { kTexFormatRGBA4444, GL_RGBA },
  { kTexFormatRGBA5551, GL_RGBA },
  { kTexFormatRGB888, GL_RGB },
  { kTexFormatRGB565, GL_RGB },
  { kTexFormatLuminance, GL_LUMINANCE },
  { kTexFormatAlpha, GL_ALPHA },
  { kTexFormatLuminanceAlpha, GL_LUMINANCE_ALPHA },
};

int TextureParameter::SearchTable(int table[][2], int count, int in_index, int out_index, int target) {
  for (int i = 0 ; i < count ; i++) {
    if (table[i][in_index] == target) {
      return table[i][out_index];
    }
  }
  return -1;  //not valid
}
GLenum TextureParameter::ConvertToGLenum(TexMinFilter orig) {
  for (int i = 0 ; i < kTexMinCount ; i++) {
    if (tex_min_filter_table[i][0] == orig) {
      return tex_min_filter_table[i][1];
    }
  }
  SR_ASSERT(!"not valid");
  return tex_min_filter_table[0][1];
}

GLenum TextureParameter::ConvertToGLenum(TexMagFilter orig) {
  int result = SearchTable(tex_mag_filter_table, kTexMagCount, 0, 1, orig);
  if (result != -1) {
    return result;
  } else {
    SR_ASSERT(!"not valid");
    return tex_mag_filter_table[0][1];
  }
}
GLenum TextureParameter::ConvertToGLenum(TexWrapMode orig) {
  int result = SearchTable(tex_wrap_mode_table, kTexWrapCount, 0, 1, orig);
  if (result != -1) {
    return result;
  } else {
    SR_ASSERT(!"not valid");
    return tex_wrap_mode_table[0][1];
  }
}
GLenum TextureParameter::ConvertToGLenum(TexFormat orig) {
  int result = SearchTable(tex_format_table, kTexFormatCount, 0, 1, orig);
  if (result != -1) {
    return result;
  } else {
    SR_ASSERT(!"not valid");
    return GL_RGBA;
  }
}
TexMinFilter TextureParameter::ConvertToTexMinFilter(GLenum orig) {
  for (int i = 0 ; i < kTexMinCount ; i++) {
    if (tex_min_filter_table[i][1] == orig) {
      return (TexMinFilter)tex_min_filter_table[i][0];
    }
  }
  SR_ASSERT(!"not valid");
  return (TexMinFilter)tex_min_filter_table[0][0];
}
TexMagFilter TextureParameter::ConvertToTexMagFilter(GLenum orig) {
  int result = SearchTable(tex_mag_filter_table, kTexMagCount, 1, 0, orig);
  if (result != -1) {
    return (TexMagFilter)result;
  } else {
    SR_ASSERT(!"not valid");
    return (TexMagFilter)tex_mag_filter_table[0][0];
  }
}
TexWrapMode TextureParameter::ConvertToTexWrapMode(GLenum orig) {
  int result = SearchTable(tex_wrap_mode_table, kTexWrapCount, 1, 0, orig);
  if (result != -1) {
    return (TexWrapMode)result;
  } else {
    SR_ASSERT(!"not valid");
    return (TexWrapMode)tex_wrap_mode_table[0][0];
  }
}
boolean TextureParameter::IsMipMap(TexMinFilter min_filter) {
  for (int i = 0 ; i < kTexMinCount ; i++) {
    if (tex_min_filter_table[i][0] == min_filter) {
      return tex_min_filter_table[i][2];
    }
  }
  SR_ASSERT(!"not valid");
  return false;
}

/////////////////////////

Texture::Texture()
  : handle(0) {
  // 초기화 코드 공유를 위해서
  Cleanup();
}
Texture::~Texture() {
  Cleanup();
}
void Texture::Cleanup() {
  param_.wrap_s = kTexWrapRepeat;
  param_.wrap_t = kTexWrapRepeat;
  param_.min_filter = kTexMinNearestMipMapOff;
  param_.mag_filter = kTexMagNearest;
  
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

  static TextureParameter default_param;
  default_param.min_filter = kTexMinNearestMipMapOff;
  default_param.mag_filter = kTexMagNearest;
  default_param.wrap_s = kTexWrapRepeat;
  default_param.wrap_t = kTexWrapRepeat;
  
  if (fmt == kTexFormatRGBA8888) {
    GLuint tex_id;
    srglGenTextures(1, &tex_id);
    srglBindTexture(GL_TEXTURE_2D, tex_id);
    srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, default_param.gl_min_filter()); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, default_param.gl_mag_filter()); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, default_param.gl_wrap_s());
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, default_param.gl_wrap_t());
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    tex->format = fmt;
    tex->handle = tex_id;
    tex->param_ = default_param;

    tex->tex_header.bpp = 32;
    tex->tex_header.src_width = 2;
    tex->tex_header.src_height = 2;
    tex->tex_header.tex_width = 2;
    tex->tex_header.tex_height = 2;

  } else {
    SR_ASSERT(!"not support yet");
  }
}

void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length);
void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length) {
	png_voidp a = png_get_io_ptr(png_ptr);
  MemoryFile *file = (MemoryFile*)a;
	file->Read((char*)data, length);
}
boolean Texture::LoadFromPNG(const char *filepath, Texture *tex) {
  LOGI("Load PNG Texture Start : %s", filepath);

  const int PngSigSize = 8;
	//png 로딩된 메모리
	png_bytep* row_ptrs = NULL;
	char *data = NULL;

	//http://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng/
	//http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams
  MemoryFile asset(filepath);
	bool opened = asset.Open();
	SR_ASSERT(opened == true);
	const char *filename = filepath;

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

	//format으로 헤더 초기화
  tex->tex_header.src_width = img_width;
  tex->tex_header.src_height = img_height;
  tex->tex_header.tex_width = img_width;
  tex->tex_header.tex_height = img_height;

  TexFormat format = kTexFormatRGBA8888;
	switch(color_type) {
	case PNG_COLOR_TYPE_PALETTE:
		//위에서 팔레트는 RGB변환했으니
    format = kTexFormatRGB888;
    tex->tex_header.bpp = 24;
		break;
	case PNG_COLOR_TYPE_GRAY:
    format = kTexFormatLuminance;
    tex->tex_header.bpp = 8;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
    format = kTexFormatLuminanceAlpha;
    tex->tex_header.bpp = 16;
		break;
	case PNG_COLOR_TYPE_RGB:
		format = kTexFormatRGB888;
    tex->tex_header.bpp = 24;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
    format = kTexFormatRGBA8888;
    tex->tex_header.bpp = 32;
		break;
	default: 
    SR_ASSERT(!"do not reach");
	}
  tex->format = format;


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
	//load complete, create texture
	//////////////////////////////////////////
	
	//텍스쳐 생성하기
	GLuint texture;
	glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //set texture default parameter;
  tex->param_.mag_filter = kTexMagLinear;
  tex->param_.min_filter = kTexMinLinearMipMapNearest;
  tex->param_.wrap_s = kTexWrapRepeat;
  tex->param_.wrap_t = kTexWrapRepeat;
	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->param_.gl_mag_filter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->param_.gl_min_filter());
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->param_.gl_wrap_s());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->param_.gl_wrap_t());
	//use mipmap
  if(tex->param_.IsMipMap()) {
	  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  }
  
  tex->handle = texture;
  

  GLenum gl_format = TextureParameter::ConvertToGLenum(format);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, img_width, img_height, 0, gl_format, GL_UNSIGNED_BYTE, (GLvoid*) data);
	
	//clean up
	//Delete the row pointers array....
	delete[]((png_bytep)row_ptrs);
	delete[](data);
	//And don't forget to clean up the read and info structs !
	png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)0);
	//////////////////////////////////

  LOGI("Load PNG Texture End");

	return tex;
}
}
