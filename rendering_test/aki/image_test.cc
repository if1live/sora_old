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
#include "aki/image.h"
#include "aki/image_description.h"
#include "aki/image_loader.h"
#include "mio/path.h"

using mio::Path;
using aki::ImageLoader;
using aki::ImageDescription;
using aki::ImagePtr;
using std::string;

TEST(AkiImage, test) {
}

TEST(AkiImageLoader, Load) {
  string path1 = Path::AppPath("testdata/aki/c.png");
  ImagePtr img = ImageLoader::Load(path1);
  const ImageDescription &desc1 = img->desc();
  EXPECT_EQ(aki::kInternalFormatRGBA, desc1.internal_format());
  EXPECT_EQ(500, desc1.width());
  EXPECT_EQ(282, desc1.height());

  string path2 = Path::AppPath("testdata/aki/target.png");
  ImagePtr img1 = ImageLoader::Load(path2);
  const ImageDescription &desc2 = img1->desc();
  EXPECT_EQ(aki::kInternalFormatRGBA, desc2.internal_format());
  EXPECT_EQ(256, desc2.width());
  EXPECT_EQ(256, desc2.height());
}
