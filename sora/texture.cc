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
GLenum Texture::ConvertToGLenum(TexFormat orig) {
  switch (orig) {
    case kTexFormatAuto:
      SR_ASSERT(!"auto???");
      return GL_RGBA;
    case kTexFormatRGBA8888:
      return GL_RGBA;
    case kTexFormatRGBA4444:
      return GL_RGBA;
    case kTexFormatRGBA5551:
      return GL_RGBA;
    case kTexFormatRGB888:
      return GL_RGB;
    case kTexFormatRGB565:
      return GL_RGB;
    case kTexFormatLuminance:
      return GL_LUMINANCE;
    case kTexFormatAlpha:
      return GL_ALPHA;
    case kTexFormatLuminanceAlpha:
      return GL_LUMINANCE_ALPHA;
    default:
      SR_ASSERT(!"do not reach");
      return GL_RGBA;
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

void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length);
void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length) {
	png_voidp a = png_get_io_ptr(png_ptr);
  MemoryFile *file = (MemoryFile*)a;
	file->Read((char*)data, length);
}
boolean Texture::LoadFromPNG(const char *filepath, Texture *tex) {
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
	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//use mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  
  tex->handle = texture;
  tex->mag_filter = kTexMagLinear;
  tex->min_filter = kTexMinLinearMipMapNearest;
  tex->wrap_s = kTexWrapRepeat;
  tex->wrap_t = kTexWrapRepeat;
  tex->mipmap = true;

  GLenum gl_format = ConvertToGLenum(format);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, img_width, img_height, 0, gl_format, GL_UNSIGNED_BYTE, (GLvoid*) data);
	
	//clean up
	//Delete the row pointers array....
	delete[]((png_bytep)row_ptrs);
	delete[](data);
	//And don't forget to clean up the read and info structs !
	png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)0);
	//////////////////////////////////
	return tex;

}
}
