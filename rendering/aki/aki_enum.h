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
#ifndef RENDERING_AKI_AKI_ENUM_H_
#define RENDERING_AKI_AKI_ENUM_H_

namespace aki {;
class Image;
class ImageDescription;
typedef std::tr1::shared_ptr<Image> ImagePtr;

// opengl을 위한 이미지라이브러리니까 opengl es에서 받아들일수있는
// 포맷으로만 내부에서 처리해도 충분하다
// opengl과 동일한 enum을 씀으로써 직통변환을 가능하게함
typedef enum {
  kInternalFormatAlpha = GL_ALPHA,
  kInternalFormatLuminance = GL_LUMINANCE,
  kInternalFormatLuminanceAlpha = GL_LUMINANCE_ALPHA,
  kInternalFormatRGB = GL_RGB,
  kInternalFormatRGBA = GL_RGBA,
} InternalFormat;

// opengl과 같은 포맷을 써서 직동변환을 허용
typedef enum {
  kPixelTypeByte = GL_UNSIGNED_BYTE,
  kPixelType4444 = GL_UNSIGNED_SHORT_4_4_4_4,
  kPixelType5551 = GL_UNSIGNED_SHORT_5_5_5_1,
  kPixelType565 = GL_UNSIGNED_SHORT_5_6_5
} PixelType;

// texture
class TextureSize;
template<unsigned int N>  class TextureGroup;
typedef TextureGroup<1> Texture;
typedef std::tr1::shared_ptr<Texture> TexturePtr;
}
#endif  // RENDERING_AKI_AKI_ENUM_H_
