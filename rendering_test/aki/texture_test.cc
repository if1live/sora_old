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
// Ŭnicode pleasep
#include "precompile.h"
#include "aki/texture2d.h"
#include "aki/image.h"
#include "mio/path.h"
#include "aki/texture_loader.h"

using std::string;
using mio::Path;
using aki::TextureSize;
using aki::TextureLoader;
using aki::Image;
using aki::ImagePtr;
using aki::Texture2D;

TEST(AkiTexture, test) {
  // load sample image
  string path1 = Path::AppPath("testdata/aki/c.png");
  ImagePtr img = Image::Create(path1);

  Texture2D tex;
  GLuint tex_id;
  TextureSize tex_size;
  TextureLoader::LoadTexture2D(*img, &tex_id, &tex_size);
  tex.SetTexture(tex_id, tex_size);
  
  // 원본 이미지 크기 정보
  EXPECT_EQ(500, img->desc().width);
  EXPECT_EQ(282, img->desc().height);

  const TextureSize &size = tex.size();
  EXPECT_EQ(500, size.orig_width);
  EXPECT_EQ(282, size.orig_height);
  EXPECT_EQ(512, size.tex_width);
  EXPECT_EQ(512, size.tex_height);
}