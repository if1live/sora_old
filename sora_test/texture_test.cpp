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
#include "sora_test_stdafx.h"
#include "sora/texture.h"

TEST(TextureParameter, ConvertEnumXX) {
  using namespace sora;
  EXPECT_EQ(GL_LINEAR_MIPMAP_LINEAR, TextureParameter::ConvertToGLenum(kTexMinLinearMipMapLinear));
  EXPECT_EQ(kTexMinLinearMipMapLinear, TextureParameter::ConvertToTexMinFilter(GL_LINEAR_MIPMAP_LINEAR));

  EXPECT_EQ(GL_LINEAR, TextureParameter::ConvertToGLenum(kTexMagLinear));
  EXPECT_EQ(kTexMagLinear, TextureParameter::ConvertToTexMagFilter(GL_LINEAR));

  EXPECT_EQ(GL_MIRRORED_REPEAT, TextureParameter::ConvertToGLenum(kTexWrapMirroredRepeat));
  EXPECT_EQ(kTexWrapMirroredRepeat, TextureParameter::ConvertToTexWrapMode(GL_MIRRORED_REPEAT));
}

TEST(TextureParameter, IsMipMap) {
  using namespace sora;
  EXPECT_EQ(true, TextureParameter::IsMipMap(kTexMinLinearMipMapLinear));
  EXPECT_EQ(true, TextureParameter::IsMipMap(kTexMinNearestMipMapLinear));
  EXPECT_EQ(false, TextureParameter::IsMipMap(kTexMinLinearMipMapOff));
  EXPECT_EQ(false, TextureParameter::IsMipMap(kTexMinNearestMipMapOff));
}