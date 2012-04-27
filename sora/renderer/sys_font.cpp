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
#include "sys_font.h"
#include "vincent.h"
#include "core/template_lib.h"
#include "texture.h"

#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

namespace sora {;
SysFont::SysFont() 
  : font_tex_("SYSTEM_FONT") {
  // 텍스쳐는 1/0만 처리하면됨. 작게 할당받자
  typedef unsigned char byte;
  int img_size = sizeof(byte) * kTextureWidth * kTextureHeight;
  byte *data = static_cast<byte*>(SR_MALLOC(img_size));
  memset(data, 0, sizeof(byte) * img_size);

  // 이미지 생성.
  for (int i = 0 ; i < 128 ; i++) {
    unsigned char *character_font = vincent_data[i];
    int base_x;
    int base_y;
    GetCharacterCoord(i, &base_x, &base_y);

    for (int y = 0 ; y < kFontSize ; y++) {
      char character_scanline = character_font[y];
      for (int x = 0 ; x < kFontSize ; x++) {
        int raw_pixel = (1 << (kFontSize - 1 - x)) & character_scanline;
        bool pixel = raw_pixel > 0 ? true : false;
        int pixel_index = GetCharacterPixelIndex(base_x, base_y, x, y);

        if(pixel == true) {
          data[pixel_index] = UCHAR_MAX;
        }
      }
    }
  }
  //create texture
  //Nearest로 써야된다. 그래야 텍스쳐 번지는게 없다
  TextureParam param;
  param.mag_filter = kTexMagNearest;
  param.min_filter = kTexMinNearest;
  font_tex_.Init();
  font_tex_.LoadTexture(data, kTextureWidth, kTextureHeight, kTexFormatAlpha, param);
  

  SR_FREE(data);
}
SysFont::~SysFont() {
  font_tex_.Deinit();
}

Texture &SysFont::font_texture() {
  return font_tex_;
}
void SysFont::Draw() const {
}
void SysFont::GetCharacterCoord(unsigned char ch, int *x, int *y) const {
  *y = ch / kColCount;
  *x = ch % kColCount;
}
int SysFont::GetCharacterPixelIndex(int base_x, int base_y, int x, int y) const {
  int pixel_x = base_x * kFontSize + x;
  int pixel_y = base_y * kFontSize + y;
  int index = pixel_y * kTextureWidth + pixel_x;
  return index;
}

void SysFont::GetCharacterTextureQuad(unsigned char ch, float *left, float *right, float *top, float *bottom) const {
  int x, y;
  GetCharacterCoord(ch, &x, &y);
  y = kRowCount - y - 1;
  *left = x * (1.0f / kColCount);
  *right = (x + 1) * (1.0f / kColCount);
  *bottom = 1 - y * (1.0f / kRowCount);
  *top = 1 - (y + 1) * (1.0f / kRowCount);
}

///////////////////////////////////
Label::Label(SysFont *font) 
  : font_(font) {
    Init("");
}
Label::Label(SysFont *font, const std::string &msg) 
  : font_(font), text_(msg) {
    Init(text_);
}
Label::~Label() {
}
void Label::Init(const std::string &text) {
  Vertex2DList &vertex_list = vert_list_;
  IndexList &index_list = index_list_;

  for (size_t i = 0 ; i < text.size() ; i++) {
    unsigned char ch = text[i];
    float tex_left, tex_right, tex_top, tex_bottom;
    font_->GetCharacterTextureQuad(ch, &tex_left, &tex_right, &tex_top, &tex_bottom);

    float left, right, top, bottom;
    left = i * (float)SysFont::kFontSize;
    right = left + (float)SysFont::kFontSize;
    top = 0.0f;
    bottom = top - (float)SysFont::kFontSize;

    // 3 2
    // 0 1
    Vertex2D v0 = CreateVertex2D(left, bottom, tex_left, tex_bottom);
    Vertex2D v1 = CreateVertex2D(right, bottom, tex_right, tex_bottom);
    Vertex2D v2 = CreateVertex2D(right, top, tex_right, tex_top);
    Vertex2D v3 = CreateVertex2D(left, top, tex_left, tex_top);

    int base_index = vertex_list.size();

    vertex_list.push_back(v0);
    vertex_list.push_back(v1);
    vertex_list.push_back(v2);
    vertex_list.push_back(v3);

    index_list.push_back(base_index + 0);
    index_list.push_back(base_index + 1);
    index_list.push_back(base_index + 2);

    index_list.push_back(base_index + 0);
    index_list.push_back(base_index + 2);
    index_list.push_back(base_index + 3);
  }
}

const Vertex2D *Label::vertex_data() const {
  if(vert_list_.empty()) {
    return NULL;
  }
  return &vert_list_[0];
}
const void *Label::index_data() const {
  if(index_list_.empty()) {
    return NULL;
  }
  return &index_list_[0];
}
int Label::vertex_count() const {
  return vert_list_.size();
}
int Label::index_count() const {
  return index_list_.size();
}

void Label::set_text(const std::string &text) {
  if(text_ == text) {
    return;
  }
  text_ = text;
  vert_list_.clear();
  index_list_.clear();
  Init(text);
}


}