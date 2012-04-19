/*  Copyright (C) 2011-2012 by if1live */
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
#include "gl_texture.h"
#include "renderer/globals.h"

#if SR_USE_PCH == 0
#include <iostream>
#include "lodepng/lodepng.h"
#include "gl_inc.h"
#endif

#include "core/math_helper.h"

#include "soil/SOIL.h"

namespace sora {;
namespace gl {
  //format별 image loader
  class PNGLoader {
  public:
    PNGLoader();
    ~PNGLoader();

    bool Decode(uchar *data, int data_size);
    const void *data() const { return &image_[0]; }

    int width() const { return width_; }
    int height() const { return height_; }
    int bit_depth() const { return bit_depth_; }
    int bpp() const { return bpp_; }
    int color_channels() const { return color_channels_; }

    void PrintLog();

  private:
    std::vector<unsigned char> image_;

    //image data
    int width_;
    int height_;
    int bit_depth_;
    int bpp_;
    int color_channels_;

    LodePNG::Decoder decoder_;
  };

  PNGLoader::PNGLoader()
    : width_(0),
    height_(0),
    bit_depth_(0),
    bpp_(0),
    color_channels_(0) {
  }

  PNGLoader::~PNGLoader() {
  }

  bool PNGLoader::Decode(uchar *data, int data_size) {
    SR_ASSERT(data != NULL);
    SR_ASSERT(data_size > 0);
    image_.clear();

    decoder_.decode(image_, data, data_size);
    if(decoder_.hasError()) { 
      return false;

    } else {
      width_ = decoder_.getWidth();
      height_ = decoder_.getHeight();
      bit_depth_ = decoder_.getInfoPng().color.bitDepth;
      bpp_ = decoder_.getBpp();
      color_channels_ = decoder_.getChannels();

      return true;
    }
  }
  void PNGLoader::PrintLog() {
    if(decoder_.hasError()) {
      std::cout << "error " << decoder_.getError() << ": " << LodePNG_error_text(decoder_.getError()) << std::endl;
    } else {
      std::cout << "\n" <<
        "w: " << decoder_.getWidth() << "\n" <<
        "h: " << decoder_.getHeight() << "\n" <<
        "bitDepth: " << decoder_.getInfoPng().color.bitDepth << "\n" <<
        "bpp: " << decoder_.getBpp() << "\n" <<
        "colorChannels: " << decoder_.getChannels() << "\n" <<
        "paletteSize: " << decoder_.getInfoPng().color.palettesize / 4 << "\n" <<
        "colorType: " << decoder_.getInfoPng().color.colorType << "\n" <<
        "compressionMethod: " << decoder_.getInfoPng().compressionMethod << "\n" <<
        "filterMethod: " << decoder_.getInfoPng().filterMethod << "\n" <<
        "interlaceMethod: " << decoder_.getInfoPng().interlaceMethod << "\n";
      for(size_t i = 0; i < decoder_.getInfoPng().text.num; i++) {
        std::cout << decoder_.getInfoPng().text.keys[i] << ": " << decoder_.getInfoPng().text.strings[i] << "\n";
      }
    }
  }

  ////////////////////////////////////////////

  Texture::Texture(const char *name, uint policy)
    : start_(NULL), 
    end_(NULL), 
    handle_(0),
    file_fmt_(kTexFileUnknown),
    tex_width_(0),
    tex_height_(0),
    src_width_(0),
    src_height_(0),
    policy_(policy),
    name_(name),
    has_alpha_(false) {
  }

  Texture::Texture(const std::string &name, uint policy)
    : start_(NULL), 
    end_(NULL), 
    handle_(0),
    file_fmt_(kTexFileUnknown),
    tex_width_(0),
    tex_height_(0),
    src_width_(0),
    src_height_(0),
    policy_(policy),
    name_(name),
    has_alpha_(false) {
  }

  Texture::~Texture() {
  }

  void Texture::Deinit() {
    if(handle_ != 0) {
      glDeleteTextures(1, &handle_);
      handle_ = 0;
    }
  }
  void Texture::SetData(TexFileType file, uchar *start, uchar *end) {
    file_fmt_ = file;
    start_ = start;
    end_ = end;
  }

  /** create sample texture
  강제로 텍스쳐 생성하는 예제
  GLubyte pixel_data[4*3] = {
  255, 0, 0,
  0, 255, 0,
  0, 0, 255,
  255, 255, 0,
  };
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLuint tex_id = -1;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  tex.Init(tex_id, 2, 2);
  */
  bool Texture::Init(GLuint tex_id, int width, int height, bool has_alpha) {
    if(Loaded() == true) {
      return false;
    }

    handle_ = tex_id;
    file_fmt_ = kTexFileUnknown;
    start_ = NULL;
    end_ = NULL;
    tex_width_ = width;
    tex_height_ = height;
    src_width_ = width;
    src_height_ = height;
    has_alpha_ = has_alpha;  //이 텍스쳐에 알파가 잇나 없나?
    return true;
  }

  bool Texture::Loaded() const {
    if(handle_ == 0) {
      return false;
    } else {
      return true;
    }
  }
  bool Texture::Init() {
    if(Loaded() == true) {
      return false;
    }

    bool result = false;

    switch(file_fmt_) {
    case kTexFileUnknown:
    case kTexFileJPEG:
      glGenTextures(1, &handle_);
      result = Load_ImageBySOIL(handle_);
      break;

    case kTexFilePNG:
      glGenTextures(1, &handle_);
      result = Load_PNG(handle_);
      break;

    default:
      result = false;
      break;
    }
    //clear data
    file_fmt_ = kTexFileUnknown;
    start_ = NULL;
    end_ = NULL;
    return result;
  }

  bool Texture::Reload(Texture &data) {
    if(Loaded() == false) {
      SetData(data.file_fmt_, data.start_, data.end_);
      bool result = Init();
      return result;
    }
    //else...reload
    SetData(data.file_fmt_, data.start_, data.end_);
    bool result = false;

    switch(file_fmt_) {
    case kTexFileUnknown:
    case kTexFileJPEG:
      result = Load_ImageBySOIL(handle_);
      break;

    case kTexFilePNG:
      result = Load_PNG(handle_);
      break;

    default:
      result = false;
      break;
    }
    //clear data
    file_fmt_ = kTexFileUnknown;
    start_ = NULL;
    end_ = NULL;
    return result;

  }

  bool Texture::Load_ImageBySOIL(GLuint tex_id) {
    //귀찮은 관계로 soil에 떠넘기자
    unsigned char *data = start_;
    int size = end_ - start_;

    //압축된거같은 데이터에서 쌩 데이터로 떠내기
    int width, height, channels;
    unsigned char *img = SOIL_load_image_from_memory(
      data, size, &width, &height, &channels, SOIL_LOAD_AUTO
      );
    if(img != NULL) {
      //텍스쳐로 생성
      GLenum format = 0;
      switch(channels) {
      case SOIL_LOAD_L:
        format = GL_LUMINANCE;
        break;
      case SOIL_LOAD_LA:
        format = GL_LUMINANCE_ALPHA;
        break;
      case SOIL_LOAD_RGB:
        format = GL_RGB;
        break;
      case SOIL_LOAD_RGBA:
        format = GL_RGBA;
        break;
      default:
        SR_ASSERT(!"do not reach");
        break;
      }
      bool result = LoadTexture(tex_id, img, width, height, format);
      SOIL_free_image_data(img);
      return result;
    } else {
      return false;
    }
  }
  bool Texture::LoadTexture(GLuint tex_id, unsigned char *image, int width, int height, GLenum format) {
    //알파 있는거/없는거 분류
    if(format == GL_RGBA || format == GL_LUMINANCE_ALPHA) {
      has_alpha_ = true;
    } else {
      has_alpha_ = false;
    }

    int color_channel_table[][2] = {
      { GL_LUMINANCE, 1 },
      { GL_LUMINANCE_ALPHA, 2 },
      { GL_RGB, 3 },
      { GL_RGBA, 4 },
    };
    int color_channel = 0;
    for(int i = 0 ; i < 4 ; ++i) {
      if(format == color_channel_table[i][0]) {
        color_channel = color_channel_table[i][1];
        break;
      }
    }
    SR_ASSERT(color_channel > 0);


    GLenum elem_type = GL_UNSIGNED_BYTE;
    src_width_ = width;
    src_height_ = height;

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //2의 승수로 되어야하는 경우, 미리 설정
    uchar *new_data = NULL;
    uchar *image_data = NULL;
    if((policy_ & kTexPolicyForcePOT) && IsPower(2, src_width_) && IsPower(2, src_height_)) {
      //2의 승수가 아니면 가상 이미지로 적절히 복사
      tex_width_ = CeilPower(2, src_width_);
      tex_height_ = CeilPower(2, src_height_);

      new_data = (uchar*)SR_MALLOC(sizeof(uchar) * tex_width_ * tex_height_ * color_channel);
      memset(new_data, 128, sizeof(uchar) * tex_width_ * tex_height_ * color_channel);
      for(int y = 0 ; y < src_height_ ; y++) {
        int new_scanline_size = tex_width_ * color_channel;
        int old_scanline_size = src_width_ * color_channel;
        int new_data_idx = y * new_scanline_size;
        int old_data_idx = y * old_scanline_size;
        uchar *new_scanline = new_data + new_data_idx;
        uchar *old_scanline = (uchar*)image + old_data_idx;
        memcpy(new_scanline, old_scanline, old_scanline_size);
      }
      image_data = new_data;
    } else {
      tex_width_ = src_width_;
      tex_height_ = src_height_;
      image_data = image;
    }

    //정책설정이 텍스쳐보다 우선되어야한다
    if(IsPower(2, tex_width_) == false || IsPower(2, tex_height_) == false) {
      //npot는 wrap로 GL_CLAMP_TO_EDGE, 만 허용한다
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      //npot는 mipmap불가능
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width_, tex_height_, 0, format, elem_type, image_data);

    //메모리 할당한 경우, 지우기
    if(new_data != NULL) {
      SR_FREE(new_data);
    }
    return true;
  }

  bool Texture::Load_PNG(GLuint tex_id) {
    int size = end_ - start_;
    PNGLoader loader;
    bool decode_result = loader.Decode(start_, size);
    if(decode_result == false) {
      return false;
    }

    GLenum format = GL_RGBA;
    if(loader.color_channels() == 4) {
      format = GL_RGBA;
    } else if(loader.color_channels() == 3) {
      format = GL_RGB;
    } else if(loader.color_channels() == 2) {
      //format = GL_LUMINANCE_ALPHA;
    } else if(loader.color_channels() == 1) {
      //format = GL_LUMINANCE;
    }

    /*
    GLenum elem_type = GL_UNSIGNED_BYTE;
    if(loader.bit_depth() == 8) {
    elem_type = GL_UNSIGNED_BYTE;
    } else {
    SR_ASSERT(!"unknowkn bit depth");
    }
    */

    int width = loader.width();
    int height = loader.height();
    unsigned char *data = (unsigned char*)loader.data();
    bool result = LoadTexture(tex_id, data, width, height, format);
    return result;
  }

} //namespace gl
} //namespace sora