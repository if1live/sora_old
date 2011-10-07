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
#ifndef RENDERING_KANAKO_FONT_H_
#define RENDERING_KANAKO_FONT_H_

#include "aki/aki_enum.h"
#include "runa/runa_enum.h"
#include "runa/vertex.h"

namespace kanako {;
class Font {
public:
  typedef std::vector<runa::TextureVertex> VertexListType;
  typedef std::vector<unsigned short> IndexListType;

public:
  Font();
  ~Font();

  void Draw() const;
  void GetCharacterCoord(unsigned char ch, int *x, int *y) const;
  int GetCharacterPixelIndex(int base_x, int base_y, int x, int y) const;
  
  void GetCharacterTextureQuad(unsigned char ch,
    float *left, float *right, float *top, float *bottom) const;
private:
  std::auto_ptr<aki::Texture> font_texture_;
};
}

#endif  // RENDERING_KANAKO_FONT_H_
