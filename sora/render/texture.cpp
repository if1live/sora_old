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
#include "texture_info.h"
#include "texture.h"
#include "sora/memory_file.h"
#include "gl_helper.h"

#include "sora/filesystem.h"

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

  set_handle(GetSampleTexture(&tex_header.tex_width, &tex_header.tex_height));
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

  set_handle(GetLoadingTexture(&tex_header.tex_width, &tex_header.tex_height));
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

  GLuint loading_tex_id = GetLoadingTexture();
  GLuint sample_tex_id = GetSampleTexture();
  if(loading_tex_id != handle() && sample_tex_id != handle()) {
    // sample텍스쳐는 공유하므로 삭제하지 않는다
    srglDeleteTextures(1, &handle_);
    set_handle(0);
  }
}
//테스트로 쓰이는 샘플 텍스쳐는 gl함수를 썡으로 해서 생성이 가능하게 하자. 이것을 통하면
//glint로된 핸들을 직접 받아서 다룰수 있으니 디버깅에 더 유리할 것이다
GLuint Texture::GetSampleTexture(int *width, int *height) {
  static bool run = false;
  static GLuint tex_id = -1;

  const int w = 2;
  const int h = 2;
  if (width) {
    *width = w;
  }
  if (height) {
    *height = h;
  }

  if (run == false) {
    run = true;

    unsigned char tex_data[] = {
      255, 0, 0, 255,
      0, 255, 0, 255,
      0, 0, 255, 255,
      255, 255, 255, 255,
    };

    srglGenTextures(1, &tex_id);
    srglBindTexture(GL_TEXTURE_2D, tex_id);
    srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  }
  return tex_id;
}

GLuint Texture::GetLoadingTexture(int *width, int *height) {
  static bool run = false;
  static GLuint tex_id = -1;
  
  if (run == false) {
    run = true;

    //텍스쳐 떄려박는건 좀 무식한거같은데...
    using std::string;
    string filename = "\\resource\\loading.png";
    //string filename = "\\res\\test.png";
    filename = sora::Filesystem::GetAppPath(filename);
    TexFormat fmt;
    TextureHeader tex_header;
    void *data = LoadPNG(filename.c_str(), &fmt, &tex_header);
    
    srglGenTextures(1, &tex_id);
    srglBindTexture(GL_TEXTURE_2D, tex_id);
    srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (width) {
      *width = tex_header.tex_width;
    }
    if (height) {
      *height = tex_header.tex_height;
    }
    SR_ASSERT(fmt == kTexFormatRGBA8888);

    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    delete[]((unsigned char *)data);
  }
  return tex_id;
}

Texture &Texture::Sample() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    tex.set_handle(GetSampleTexture(&tex.tex_header.tex_width, &tex.tex_header.tex_height));
    tex.tex_header.src_width = tex.tex_header.tex_width;
    tex.tex_header.src_height = tex.tex_header.tex_height;
    tex.format = kTexFormatRGB888;
  }
  return tex;
}

Texture &Texture::White() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    ColorTexture(255, 255, 255, &tex);
  }
  return tex;
}
Texture &Texture::Black() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    ColorTexture(0, 0, 0, &tex);
  }
  return tex;
}
Texture &Texture::Gray() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    ColorTexture(128, 128, 128, &tex);
  }
  return tex;
}
Texture &Texture::Red() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    ColorTexture(255, 0, 0, &tex);
  }
  return tex;
}
Texture &Texture::Green() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    ColorTexture(0, 255, 0, &tex);
  }
  return tex;
}
Texture &Texture::Blue() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    ColorTexture(0, 0, 255, &tex);
  }
  return tex;
}

Texture &Texture::LoadingTexture() {
  static bool run = false;
  static Texture tex;
  if (run == false) {
    run = true;
    tex.set_handle(GetLoadingTexture(&tex.tex_header.tex_width, &tex.tex_header.tex_height));
    tex.tex_header.src_width = tex.tex_header.tex_width;
    tex.tex_header.src_height = tex.tex_header.tex_height;
    tex.format = kTexFormatRGB888;
  }
  return tex;
}

void Texture::ColorTexture(u8 r, u8 g, u8 b, Texture *tex) {
  unsigned char tex_data[] = {
    r, g, b, 255,
    r, g, b, 255,
    r, g, b, 255,
    r, g, b, 255,
  };
  tex->InitSimpleTexture(2, 2, kTexFormatRGBA8888, tex_data);
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

void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length);
void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length) {
	png_voidp a = png_get_io_ptr(png_ptr);
  MemoryFile *file = (MemoryFile*)a;
	file->Read((char*)data, length);
}
boolean Texture::LoadFromPNG(const char *filepath, Texture *tex) {
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

  LOGI("Load PNG Texture End");
	return true;
}
void* Texture::LoadPNG(const char *filepath, TexFormat *fmt, TextureHeader *tex_header) {
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
bool Texture::IsSystemTexture() const {
  using std::set;

  static set<u32> system_tex_list;
  static bool run = false;
  if (run == false) {
    run = true;
    system_tex_list.insert(GetSampleTexture());
    system_tex_list.insert(GetLoadingTexture());
    system_tex_list.insert(White().handle());
    system_tex_list.insert(Black().handle());
    system_tex_list.insert(Gray().handle());
    system_tex_list.insert(Red().handle());
    system_tex_list.insert(Blue().handle());
    system_tex_list.insert(Green().handle());
    system_tex_list.insert(Sample().handle());
    system_tex_list.insert(LoadingTexture().handle());
  }

  set<u32>::iterator found = system_tex_list.find(handle());
  if (found == system_tex_list.end()) {
    return false;
  } else {
    return true;
  }
}
}

