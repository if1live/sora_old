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
  void GLTexture::Deinit(HandleType *handle) {
    GLuint &tex_id = *handle;
    if(tex_id != 0) {
      glDeleteTextures(1, handle);
      tex_id = 0;
    }
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
  bool GLTexture::Loaded(const HandleType &handle) const {
    if(handle == 0) {
      return false;
    } else {
      return true;
    }
  }
  bool GLTexture::Init(HandleType *handle) {
    if(Loaded(*handle) == true) {
      return false;
    }
    glGenTextures(1, handle);
    return true;
  }

  void GLTexture::ApplyTextureParam(HandleType handle, const TextureParam &param) {
    //change tex filter
    GLenum mag_filter = GLEnv::TypeToGLEnum(param.mag_filter);
    GLenum min_filter = GLEnv::TypeToGLEnum(param.min_filter);
    GLenum wrap_s = GLEnv::TypeToGLEnum(param.wrap_s);
    GLenum wrap_t = GLEnv::TypeToGLEnum(param.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  }

  bool GLTexture::LoadTexture(HandleType handle, unsigned char *image, const ImageDesc &img_desc, uint policy) {
    int w = img_desc.width;
    int h = img_desc.height;
    GLenum format = GL_ALPHA;
    //enum gray alpha
    int tex_table[][3] = {
      { GL_ALPHA, false, true },
      { GL_LUMINANCE, true, false},
      { GL_LUMINANCE_ALPHA, true, true },
      { GL_RGB, false, false},
      { GL_RGBA, false, true },
    };
    const int tex_table_size = sizeof(tex_table) / sizeof(tex_table[0]);
    for(int i = 0 ; i < tex_table_size ; i++) {
      if(tex_table[i][1] == (int)img_desc.is_grayscale && tex_table[i][2] == (int)img_desc.is_alpha) {
        format = tex_table[i][0];
        break;
      }
    }
    return LoadTexture(handle, image, w, h, format, policy);
  }

  bool GLTexture::LoadTexture(GLuint tex_id, unsigned char *image, int width, int height, GLenum format, uint policy) {  
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
    int src_width = width;
    int src_height = height;
    int tex_width = width;
    int tex_height = height;

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //2의 승수로 되어야하는 경우, 미리 설정
    uchar *new_data = NULL;
    uchar *image_data = image;
    if((policy & kTexPolicyForcePOT) && IsPower(2, src_width) && IsPower(2, src_height)) {
      //2의 승수가 아니면 가상 이미지로 적절히 복사
      int tex_width = CeilPower(2, src_width);
      int tex_height = CeilPower(2, src_height);

      new_data = (uchar*)SR_MALLOC(sizeof(uchar) * tex_width * tex_height * color_channel);
      memset(new_data, 128, sizeof(uchar) * tex_width * tex_height * color_channel);
      for(int y = 0 ; y < src_height ; y++) {
        int new_scanline_size = tex_width * color_channel;
        int old_scanline_size = src_width * color_channel;
        int new_data_idx = y * new_scanline_size;
        int old_data_idx = y * old_scanline_size;
        uchar *new_scanline = new_data + new_data_idx;
        uchar *old_scanline = (uchar*)image + old_data_idx;
        memcpy(new_scanline, old_scanline, old_scanline_size);
      }
      image_data = new_data;
    }

    //정책설정이 텍스쳐보다 우선되어야한다
    if(IsPower(2, tex_width) == false || IsPower(2, tex_height) == false) {
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
    glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, elem_type, image_data);

    //메모리 할당한 경우, 지우기
    if(new_data != NULL) {
      SR_FREE(new_data);
    }
    return true;
  }

  bool GLTexture::LoadTexture(const HandleType &handle, const Image &image, uint policy) {
    unsigned char *data = const_cast<unsigned char*>(image.data());
    int w = image.width();
    int h = image.height();

    GLenum format = GL_RGBA;
    if(image.color_channels() == 4) {
      format = GL_RGBA;
    } else if(image.color_channels() == 3) {
      format = GL_RGB;
    } else if(image.color_channels() == 2) {
      //format = GL_LUMINANCE_ALPHA;
    } else if(image.color_channels() == 1) {
      //format = GL_LUMINANCE;
    }
    bool result = LoadTexture(handle, data, w, h, format, policy);
    return result;
  }
} //namespace gl
} //namespace sora