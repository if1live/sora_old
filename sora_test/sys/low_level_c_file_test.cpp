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
#include "sora/sys/low_level_c_file.h"

#include "sora/sys/memory_file.h"

TEST(LowLevelCFile, test) {
  using sora::LowLevelCFile;
  const char *path1 = "low_level_c_file.txt";

  LowLevelCFile file1(path1);
  EXPECT_EQ(false, file1.IsOpened());
  ASSERT_EQ(true, file1.Open("rb"));
  
  EXPECT_EQ(28, file1.GetLength());
  const char *content = "this is line1.this is line2.";
  const char *buffer = (const char *)file1.GetBuffer();
  EXPECT_STREQ(content, buffer);
}

TEST(ReadonlyFile_WriteonlyFile, test) {
  using sora::WriteonlyCFile;
  using sora::ReadonlyCFile;
  const char path1[] = "asdf.txt";
  const char content[] = "this is line1.this is line2.";

  WriteonlyCFile file1(path1);
  EXPECT_EQ(false, file1.IsOpened());
  ASSERT_EQ(true, file1.Open());
  file1.Write(content, sizeof(content));
  file1.Close();

  ReadonlyCFile file2(path1);
  EXPECT_EQ(false, file2.IsOpened());
  EXPECT_EQ(true, file2.Open());
  EXPECT_EQ(sizeof(content), file2.GetLength());
  const char *buffer = (const char *)file2.GetBuffer();
  EXPECT_STREQ(content, buffer);
}

TEST(MemoryFile, test) {
  using sora::MemoryFile;
  const char *path1 = "low_level_c_file.txt";

  MemoryFile file1(path1);
  EXPECT_EQ(false, file1.IsOpened());
  ASSERT_EQ(true, file1.Open());
  
  EXPECT_EQ(28, file1.GetLength());
  const char *content = "this is line1.this is line2.";
  EXPECT_STREQ(content, (const char*)file1.start);
}
