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
#include "sora_test_stdafx.h"
#include "obj_loader.h"
#include "memory_file.h"
#include "filesystem.h"
#include "obj_model.h"

using namespace std;
using namespace sora;

TEST(ObjLoader, NewLineToNullChar) {
  unsigned char str1[] = "asdf\nqwer\r\n1234";
  uchar *start = str1;
  uchar *end = start + strlen((const char*)str1);

  ObjLoader::NewLineToNullChar(start, end);
  unsigned char str2[] = "asdf\0qwer\r\01234";
  EXPECT_STREQ((char*)str2, (char*)str1);
}

TEST(ObjLoader, GetLineStartPos) {
  {
    //가장 간단한 형태
    unsigned char str1[] = "a\nb\n12";
    uchar *start = str1;
    uchar *end = start + strlen((const char*)str1);
    ObjLoader::NewLineToNullChar(start, end);

    vector<uchar*> line_list;
    int line_count = ObjLoader::GetLineStartPos(start, end, line_list);
    ASSERT_EQ(3, line_count);
    ASSERT_EQ(start, line_list[0]);
    ASSERT_EQ(start+2, line_list[1]);
    ASSERT_EQ(start+4, line_list[2]);
  }
  {
    //중간에 연속 개행이 존재하는 경우, 빈줄은 씹는다
    unsigned char str1[] = "a\n\nb\n\n\n\n12";
    uchar *start = str1;
    uchar *end = start + strlen((const char*)str1);
    ObjLoader::NewLineToNullChar(start, end);

    vector<uchar*> line_list;
    int line_count = ObjLoader::GetLineStartPos(start, end, line_list);
    ASSERT_EQ(3, line_count);
  }
}

TEST(ObjLoader, GetLineLength) {
  {
    //가장 간단한 형태
    unsigned char str1[] = "a\nb\n12";
    uchar *start = str1;
    uchar *end = start + strlen((const char*)str1);
    ObjLoader::NewLineToNullChar(start, end);

    vector<uchar*> line_list;
    vector<int> length_list;
    int line_count = ObjLoader::GetLineStartPos(start, end, line_list);
    ObjLoader::GetLineLength(line_list, length_list);
    EXPECT_EQ(line_count, length_list.size());
    ASSERT_EQ(1, length_list[0]);
    ASSERT_EQ(1, length_list[1]);
    ASSERT_EQ(2, length_list[2]);
  }
  {
    //중간에 연속 개행이 존재하는 경우, 빈줄은 씹는다
    unsigned char str1[] = "a\n\nb\n\n\n\n12";
    uchar *start = str1;
    uchar *end = start + strlen((const char*)str1);
    ObjLoader::NewLineToNullChar(start, end);

    vector<uchar*> line_list;
    vector<int> length_list;
    int line_count = ObjLoader::GetLineStartPos(start, end, line_list);
    ObjLoader::GetLineLength(line_list, length_list);
    EXPECT_EQ(line_count, length_list.size());
  }
}

TEST(ObjLoader, String_Create) {
  char str1[] = "abc\0\0\0\0";
  //std::string할떄 NULL이 추가로 붙어도 씹고 적절히 만들어낸다
  string str(str1, 6);
  EXPECT_STREQ("abc", str.c_str());
}

TEST(ObjLoader, Cube_Load) {
  using sora::MemoryFile;
  string path1 = Filesystem::GetAppPath("obj/cube.obj");
  MemoryFile file1(path1);
  EXPECT_EQ(false, file1.IsOpened());
  ASSERT_EQ(true, file1.Open());
  
  ObjModel model;
  ObjLoader loader;
  loader.LoadObj(file1.start, file1.end, &model);

}