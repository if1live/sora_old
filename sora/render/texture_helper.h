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
#ifndef SORA_TEXTURE_HELPER_H_
#define SORA_TEXTURE_HELPER_H_

namespace sora {;
class Texture;

struct TextureHelperImpl;
class TextureHelper {
public:
  static GLuint GetSampleTexture(int *width = NULL, int *height = NULL);
  static GLuint GetLoadingTexture(int *width = NULL, int *height = NULL);
  
  //테스트용 텍스쳐는 색깔별로
  static GLuint &White();
  static GLuint &Black();
  static GLuint &Gray();
  static GLuint &Red();
  static GLuint &Blue();
  static GLuint &Green();

public:
  static void Unload();
  static void Load();

  // loading, sample, color texture 같이 시스템을 위해서 만든 텍스쳐인가?
  // 자체적으로 구현한 텍스쳐인가?
  static bool IsSystemTexture(const Texture &tex);

private:
  static void ColorTexture(u8 r, u8 g, u8 b, GLuint *tex);

private:
  static TextureHelperImpl &impl();

};
}

#endif  // SORA_TEXTURE_HELPER_H_