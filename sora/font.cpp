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
#include "font.h"
#include "vincent.h"
#include "template_lib.h"
#include "texture.h"

#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

namespace sora {;
struct FontImpl {
  FontImpl() {}
  Texture font_tex; //수동으로 생성한 텍스쳐를 관리하기 위한 용도
};

Font::Font() : impl(new FontImpl()) {
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

  //create gl texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLuint tex_id = -1;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, kTextureWidth, kTextureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  impl->font_tex.Init(tex_id, kTextureWidth, kTextureHeight);

  SR_FREE(data);
}
Font::~Font() {
  impl->font_tex.Deinit();
  SafeDeleteWithNullCheck(impl);
}

Texture &Font::font_texture() {
  return impl->font_tex;
}
void Font::Draw() const {
}
void Font::GetCharacterCoord(unsigned char ch, int *x, int *y) const {
  *y = ch / kColCount;
  *x = ch % kColCount;
}
int Font::GetCharacterPixelIndex(int base_x, int base_y, int x, int y) const {
  int pixel_x = base_x * kFontSize + x;
  int pixel_y = base_y * kFontSize + y;
  int index = pixel_y * kTextureWidth + pixel_x;
  return index;
}

void Font::GetCharacterTextureQuad(unsigned char ch, float *left, float *right, float *top, float *bottom) const {
  int x, y;
  GetCharacterCoord(ch, &x, &y);
  y = kRowCount - y - 1;
  *left = x * (1.0f / kColCount);
  *right = (x + 1) * (1.0f / kColCount);
  *bottom = 1 - y * (1.0f / kRowCount);
  *top = 1 - (y + 1) * (1.0f / kRowCount);
}

///////////////////////////////////
struct LabelImpl {
  std::string text;

  std::vector<Vertex2D> vert_list;
  std::vector<ushort> index_list;
};

Label::Label() : impl(new LabelImpl()) {
}
Label::Label(const std::string &msg) : impl(new LabelImpl()) {
  impl->text = msg;
  Init(impl->text);
}
Label::Label(const char *msg) : impl(new LabelImpl()) {
  impl->text = msg;
  Init(impl->text);
}
Label::~Label() {
  SafeDeleteWithNullCheck(impl);
}
void Label::Init(const std::string &text) {
  Font &font = Font::GetInstance();

  Font::VertexListType &vertex_list = impl->vert_list;
  Font::IndexListType &index_list = impl->index_list;

  for (size_t i = 0 ; i < text.size() ; i++) {
    unsigned char ch = text[i];
    float tex_left, tex_right, tex_top, tex_bottom;
    font.GetCharacterTextureQuad(ch, &tex_left, &tex_right, &tex_top, &tex_bottom);

    float left, right, top, bottom;
    left = i * (float)Font::kFontSize;
    right = left + (float)Font::kFontSize;
    top = 0.0f;
    bottom = top - (float)Font::kFontSize;

    // 3 2
    // 0 1
    Vertex2D v0(left, bottom, tex_left, tex_bottom);
    Vertex2D v1(right, bottom, tex_right, tex_bottom);
    Vertex2D v2(right, top, tex_right, tex_top);
    Vertex2D v3(left, top, tex_left, tex_top);

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
  if(impl->vert_list.empty()) {
    return NULL;
  }
  return &impl->vert_list[0];
}
const void *Label::index_data() const {
  if(impl->index_list.empty()) {
    return NULL;
  }
  return &impl->index_list[0];
}
int Label::vertex_count() const {
  return impl->vert_list.size();
}
int Label::index_count() const {
  return impl->index_list.size();
}

void Label::set_text(const std::string &text) {
  if(impl->text == text) {
    return;
  }
  impl->text = text;
  impl->vert_list.clear();
  impl->index_list.clear();
  Init(text);
}


}