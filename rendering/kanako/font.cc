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
#include "precompile.h"
#include "kanako/font.h"
#include "kanako/vincent.h"
#include "aki/image.h"
#include "aki/image_description.h"
#include "aki/texture2d.h"
#include "aki/texture_loader.h"
#include "matsu/matrix.h"
#include "matsu/vector.h"
#include "runa/window.h"

#include "runa/gl_tool.h"
#include "runa/basic_texture_shader.h"

using std::auto_ptr;
using aki::Texture2D;
using runa::BasicTextureShader;

namespace kanako {;
Font::Font() {  
  // 텍스쳐는 1/0만 처리하면됨. 작게 할당받자
  typedef unsigned char byte;
  int img_size = sizeof(byte) * kTextureWidth * kTextureHeight;
  byte *data = static_cast<byte*>(malloc(img_size));
  memset(data, 0, sizeof(byte) * img_size);

  // 이미지 생성.
  for (int i = 0 ; i < 128 ; i++) {
    char *character_font = vincent_data[i];
    int base_x;
    int base_y;
    GetCharacterCoord(i, &base_x, &base_y);

    for (int y = 0 ; y < kFontSize ; y++) {
      char character_scanline = character_font[y];
      for (int x = 0 ; x < kFontSize ; x++) {
        bool pixel = (1 << (kFontSize - 1 - x)) & character_scanline;
        SR_ASSERT(pixel == 0 || pixel == 1);
        int pixel_index = GetCharacterPixelIndex(base_x, base_y, x, y);
        
        if(pixel == true) {
          data[pixel_index] = UCHAR_MAX;
        }
      }
    }
  }
  // create textrue from image
  aki::ImageDescription img_desc;
  img_desc.width = kTextureWidth;
  img_desc.height = kTextureHeight;
  img_desc.pixel_type = aki::kPixelTypeByte;
  img_desc.internal_format = aki::kInternalFormatLuminance;
  aki::Image img(img_desc, data);   // data의 메모리 관리책임까지 가져감
  img.InverseY();   //이미지를 뒤집어야 계산이 쉽다
  font_texture_.reset(new Texture2D());

  GLuint tex_id;
  aki::TextureSize tex_size;
  aki::TextureLoader::LoadTexture2D(img, &tex_id, &tex_size);
  font_texture_->SetTexture(tex_id, tex_size);


  // init shader
  BasicTextureShader &shader = BasicTextureShader::GetInstance();
  if(shader.IsInitialized() == false) {
    shader.Initialize();
  }
}

Font::~Font() {
}

void Font::Draw() const {
  BasicTextureShader &shader = BasicTextureShader::GetInstance();
  GLint position_location = shader.position_location();
  GLint texcoord_location = shader.texcoord_location();

  glEnable(GL_TEXTURE_2D);
  shader.Use();
  font_texture_->Bind();

  // apply projection
  matsu::mat4 projection;
  projection = matsu::Matrix::Ortho<float>(0, 512, 0, 512, 0.1f, 10.0f);
  projection *= matsu::Matrix::Translate<float>(0, 0, -1);
  //projection *= matsu::Matrix::Scale<float>(3, 3, 1);
  shader.SetMatrix(projection.Pointer());

  //색 설정
  shader.SetColorToWhite();

  //3 2
  //0 1
  int vertex[] = {
    0, 0,
    8, 0,
    8, 8,
    0, 8,
  };
  glVertexAttribPointer(position_location, 2, GL_INT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(position_location);

  //텍스쳐 설정
  float left, right, top, bottom;
  GetCharacterTextureQuad('A', &left, &right, &top, &bottom);

  float texcoord[] = {
    left, bottom,
    right, bottom,
    right, top,
    left, top
  };
  glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
  glEnableVertexAttribArray(texcoord_location);

  //index써서 그리기
  unsigned char index[] = {
    0, 1, 2,
    0, 2, 3,
  };
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, index);
  glDisable(GL_TEXTURE_2D);

  SR_ASSERT(runa::GLTool::CheckError("draw font"));
}
void Font::GetCharacterTextureQuad(unsigned char ch,
    float *left, float *right, float *top, float *bottom) const {
  int x, y;
  GetCharacterCoord(ch, &x, &y);

  *left = x * (1 / 16.0f);
  *right = (x + 1) * (1 / 16.0f);
  *top = 1 - y * (1 / 16.0f);
  *bottom = 1 - (y + 1) * (1 / 16.0f);
}
void Font::GetCharacterCoord(unsigned char ch, int *x, int *y) const {
  *y = ch / 16;
  *x = ch % 16;
}
int Font::GetCharacterPixelIndex(int base_x, int base_y, int x, int y) const {
  int pixel_x = base_x * kFontSize + x;
  int pixel_y = base_y * kFontSize + y;
  int index = pixel_y * kTextureWidth + pixel_x;
  return index;
}
void Font::BindFontTexture() const {
  font_texture_->Bind();
}

}