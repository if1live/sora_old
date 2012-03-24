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
#include "sora/sys/filesystem.h"

TEST(Filesystem, GetExtension) {
  using std::string;
  using sora::Filesystem;

  const char file1[] = "asdf.txt";
  EXPECT_STREQ("txt", Filesystem::GetExtension(file1).c_str());
  const char file2[] = "ad/sdf/asdf.txt";
  EXPECT_STREQ("txt", Filesystem::GetExtension(file2).c_str());
  const char file3[] = "ad\\sdf\\asdf.txt";
  EXPECT_STREQ("txt", Filesystem::GetExtension(file3).c_str());
  const char file4[] = "sdf";
  EXPECT_STREQ("", Filesystem::GetExtension(file4).c_str());
  const char file5[] = "ad/sd.f/asdf.txt";
  EXPECT_STREQ("txt", Filesystem::GetExtension(file5).c_str());
  const char file6[] = "ad/sd.f/asdf";
  EXPECT_STREQ("", Filesystem::GetExtension(file6).c_str());
}