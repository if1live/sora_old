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
#ifndef SORA_SYS_FONT_H_
#define SORA_SYS_FONT_H_

#include "core/vertex.h"
#if SR_USE_PCH == 0
#include <vector>
#include <string>
#endif 

#include "texture.h"

namespace sora {;
class Font;
class Label;

struct FontImpl;
class SysFont {
public:
  // vincent 폰트를 기반으로 텍스쳐 만들기
  // 128글자, 8*8폰트니까
  // 256글자를 저장할수있는 영역을 할당하고
  // 16 * 16개의 글자를 배열할수있도록 텍스쳐의 크기를
  // 128 * 128의 텍스쳐 영역에 글자를 적절히 배치한다
  enum {
    kTextureWidth = 128,
    kTextureHeight = 64,
    kFontSize = 8,
    kRowCount = kTextureHeight / kFontSize,
    kColCount = kTextureWidth / kFontSize,
  };

public:
  SysFont();
  ~SysFont();
  
  void Draw() const;
  void GetCharacterCoord(unsigned char ch, int *x, int *y) const;
  int GetCharacterPixelIndex(int base_x, int base_y, int x, int y) const;
  
  void GetCharacterTextureQuad(unsigned char ch,
    float *left, float *right, float *top, float *bottom) const;

  Texture &font_texture();

private:
  Texture font_tex_; //수동으로 생성한 텍스쳐를 관리하기 위한 용도
};

class Label {
public:
  Label(SysFont *font);
  Label(SysFont *font, const std::string &msg);
  ~Label();

  void set_text(const std::string &text);
  void set_text(const char *text) { set_text(std::string(text)); }

  const Vertex2D *vertex_data() const;
  const void *index_data() const;
  const Vertex2DList &vertex_list() const { return vert_list_; }
  const IndexList &index_list() const { return index_list_; }
  int vertex_count() const;
  int index_count() const;

private:
  void Init(const std::string &text);

private:
  std::string text_;
  Vertex2DList vert_list_;
  IndexList index_list_;
  SysFont *font_;
};

}

#endif  // SORA_FONT_H_