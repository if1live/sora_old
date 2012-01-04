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

namespace sora {;

///////////////
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
}
