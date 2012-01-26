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
#include "gl_inc.h"
#include "texture_helper.h"

#include "texture.h"
#include "sora/io/filesystem.h"

#if SR_USE_PCH == 0
#include <set>
#include <boost/foreach.hpp>
#include <vector>
#endif

namespace sora {;
struct TextureHelperImpl {
  TextureHelperImpl() 
    : sample_tex_id(0),
    loading_tex_id(0),
    white_tex_id(0),
    black_tex_id(0),
    gray_tex_id(0),
    red_tex_id(0),
    green_tex_id(0),
    blue_tex_id(0),
    red_rex_id(0),
    gree_tex_id(0) {}

  GLuint sample_tex_id;
  GLuint loading_tex_id;
  
  GLuint white_tex_id;
  GLuint black_tex_id;
  GLuint gray_tex_id;
  GLuint red_tex_id;
  GLuint green_tex_id;
  GLuint blue_tex_id;
  GLuint red_rex_id;
  GLuint gree_tex_id;
};

TextureHelperImpl &TextureHelper::impl() {
  static TextureHelperImpl ctx;
  return ctx;
}

//테스트로 쓰이는 샘플 텍스쳐는 gl함수를 썡으로 해서 생성이 가능하게 하자. 이것을 통하면
//glint로된 핸들을 직접 받아서 다룰수 있으니 디버깅에 더 유리할 것이다
GLuint TextureHelper::GetSampleTexture(int *width, int *height) {
  const int w = 2;
  const int h = 2;
  if (width) {
    *width = w;
  }
  if (height) {
    *height = h;
  }

  TextureHelperImpl &ctx = impl();
  if(ctx.sample_tex_id == 0) {
    unsigned char tex_data[] = {
      255, 0, 0, 255,
      0, 255, 0, 255,
      0, 0, 255, 255,
      255, 255, 255, 255,
    };

    srglGenTextures(1, &ctx.sample_tex_id);
    srglBindTexture(GL_TEXTURE_2D, ctx.sample_tex_id);
    srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  }
  return ctx.sample_tex_id;
}


GLuint TextureHelper::GetLoadingTexture(int *width, int *height) {
  TextureHelperImpl &ctx = impl();
  if(ctx.loading_tex_id == 0) {
    //텍스쳐 떄려박는건 좀 무식한거같은데...
    using std::string;
    string filename = "\\resource\\loading.png";
    //string filename = "\\res\\test.png";
    filename = sora::Filesystem::GetAppPath(filename);
    TexFormat fmt;
    TextureHeader tex_header;
    
    void *data = Texture::LoadPNG(filename.c_str(), &fmt, &tex_header);
    srglGenTextures(1, &ctx.loading_tex_id);
    srglBindTexture(GL_TEXTURE_2D, ctx.loading_tex_id);
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
    srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_header.tex_width, tex_header.tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[]((unsigned char *)data);

    LOGI("Loading Texture create complete");
  }
  return ctx.loading_tex_id;
}


GLuint &TextureHelper::White() {
  TextureHelperImpl &ctx = impl();
  GLuint &tex = ctx.white_tex_id;
  if(tex == 0) {
    ColorTexture(255, 255, 255, &tex);
  }
  return tex;
}
GLuint &TextureHelper::Black() {
  TextureHelperImpl &ctx = impl();
  GLuint &tex = ctx.black_tex_id;
  if(tex == 0) {
    ColorTexture(0, 0, 0, &tex);
  }
  return tex;
}
GLuint &TextureHelper::Gray() {
  TextureHelperImpl &ctx = impl();
  GLuint &tex = ctx.gray_tex_id;
  if(tex == 0) {
    ColorTexture(128, 128, 128, &tex);
  }
  return tex;
}
GLuint &TextureHelper::Red() {
  TextureHelperImpl &ctx = impl();
  GLuint &tex = ctx.red_tex_id;
  if(tex == 0) {
    ColorTexture(255, 0, 0, &tex);
  }
  return tex;
}
GLuint &TextureHelper::Green() {
  TextureHelperImpl &ctx = impl();
  GLuint &tex = ctx.green_tex_id;
  if(tex == 0) {
    ColorTexture(0, 255, 0, &tex);
  }
  return tex;
}
GLuint &TextureHelper::Blue() {
  TextureHelperImpl &ctx = impl();
  GLuint &tex = ctx.blue_tex_id;
  if(tex == 0) {
    ColorTexture(0, 0, 255, &tex);
  }
  return tex;
}


void TextureHelper::ColorTexture(u8 r, u8 g, u8 b, GLuint *tex) {
  unsigned char tex_data[] = {
    r, g, b, 255,
    r, g, b, 255,
    r, g, b, 255,
    r, g, b, 255,
  };

  srglGenTextures(1, tex);
  srglBindTexture(GL_TEXTURE_2D, *tex);
  srglPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
  srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  srglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  srglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

}

void TextureHelper::Unload() {
  TextureHelperImpl &ctx = impl();

  std::vector<GLuint*> tex_id_list;
  tex_id_list.push_back(&ctx.sample_tex_id);
  tex_id_list.push_back(&ctx.loading_tex_id);
  tex_id_list.push_back(&ctx.white_tex_id);
  tex_id_list.push_back(&ctx.black_tex_id);
  tex_id_list.push_back(&ctx.gray_tex_id);
  tex_id_list.push_back(&ctx.red_tex_id);
  tex_id_list.push_back(&ctx.green_tex_id);
  tex_id_list.push_back(&ctx.blue_tex_id);
  tex_id_list.push_back(&ctx.red_rex_id);
  tex_id_list.push_back(&ctx.gree_tex_id);

  BOOST_FOREACH(GLuint *tex, tex_id_list) {
    srglDeleteTextures(1, tex);
    *tex = 0;
  }
}
void TextureHelper::Load() {
}
bool TextureHelper::IsSystemTexture(const Texture &tex) {
  using std::set;
  
  set<u32> system_tex_list;
  system_tex_list.insert(GetSampleTexture());
  system_tex_list.insert(GetLoadingTexture());
  system_tex_list.insert(White());
  system_tex_list.insert(Black());
  system_tex_list.insert(Gray());
  system_tex_list.insert(Red());
  system_tex_list.insert(Blue());
  system_tex_list.insert(Green());


  set<u32>::iterator found = system_tex_list.find(tex.handle());
  if (found == system_tex_list.end()) {
    return false;
  } else {
    return true;
  }
}
}