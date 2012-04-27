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
#include "gl_inc.h"
#endif

#include "core/math_helper.h"

#include "soil/SOIL.h"
#include "gl_env.h"
#include "renderer/image.h"

namespace sora {;
namespace gl {
  GLTexture::GLTexture(const char *name, uint policy)
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
    has_alpha_(false),
    is_render_to_texture_(false) {
  }

  GLTexture::GLTexture(const std::string &name, uint policy)
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
    has_alpha_(false),
    is_render_to_texture_(false) {
  }

  GLTexture::~GLTexture() {
  }

  void GLTexture::Deinit() {
    if(handle_ != 0) {
      glDeleteTextures(1, &handle_);
      handle_ = 0;
    }
  }
  void GLTexture::SetData(TexFileType file, uchar *start, uchar *end) {
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
  glGenGLTextures(1, &tex_id);
  glBindGLTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  tex.Init(tex_id, 2, 2);
  */
  bool GLTexture::Init(GLuint tex_id, int width, int height, bool has_alpha, bool is_rtt) {
    if(Loaded() == true) {
      return false;
    }

    is_render_to_texture_ = is_rtt;
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

  bool GLTexture::Loaded() const {
    if(handle_ == 0) {
      return false;
    } else {
      return true;
    }
  }
  bool GLTexture::Init() {
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

  bool GLTexture::Reload(GLTexture &data) {
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

  bool GLTexture::Load_ImageBySOIL(GLuint tex_id) {
    //귀찮은 관계로 soil에 떠넘기자
    unsigned char *data = start_;
    int size = end_ - start_;
    is_render_to_texture_ = false;

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

  bool GLTexture::LoadTexture(unsigned char *image, int w, int h, TexFormatType format, const TextureParam &param) {
    GLenum gl_format = GLEnv::TypeToGLEnum(format);
    if(handle_ == 0) {
      glGenTextures(1, &handle_);
    }
    bool result = LoadTexture(handle_, image, w, h, gl_format);
    //change tex filter
    GLenum mag_filter = GLEnv::TypeToGLEnum(param.mag_filter);
    GLenum min_filter = GLEnv::TypeToGLEnum(param.min_filter);
    GLenum wrap_s = GLEnv::TypeToGLEnum(param.wrap_s);
    GLenum wrap_t = GLEnv::TypeToGLEnum(param.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    return result;
  }

  bool GLTexture::LoadTexture(GLuint tex_id, unsigned char *image, int width, int height, GLenum format) {
    //알파 있는거/없는거 분류
    if(format == GL_RGBA || format == GL_LUMINANCE_ALPHA || format == GL_ALPHA) {
      has_alpha_ = true;
    } else {
      has_alpha_ = false;
    }
  
    int color_channel_table[][2] = {
      { GL_ALPHA, 1}, 
      { GL_LUMINANCE, 1 },
      { GL_LUMINANCE_ALPHA, 2 },
      { GL_RGB, 3 },
      { GL_RGBA, 4 },
    };
    int color_channel = 0;
    for(size_t i = 0 ; i < sizeof(color_channel_table) / sizeof(color_channel_table[0]) ; ++i) {
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

  bool GLTexture::Load_PNG(GLuint tex_id) {
    Image img;
    int size = end_ - start_;
    bool load_result = img.LoadPNG(start_, size);
    if(load_result == false) {
      return false;
    }

    GLenum format = GL_RGBA;
    if(img.color_channels() == 4) {
      format = GL_RGBA;
    } else if(img.color_channels() == 3) {
      format = GL_RGB;
    } else if(img.color_channels() == 2) {
      //format = GL_LUMINANCE_ALPHA;
    } else if(img.color_channels() == 1) {
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
    is_render_to_texture_ = false;
    int width = img.width();
    int height = img.height();
    unsigned char *data = (unsigned char*)img.data();
    bool result = LoadTexture(tex_id, data, width, height, format);
    return result;
  }
} //namespace gl
} //namespace sora