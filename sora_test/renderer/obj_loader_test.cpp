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
#include "renderer/obj_loader.h"
#include "sys/memory_file.h"
#include "sys/filesystem.h"
#include "renderer/obj_model.h"
#include "renderer/material.h"

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

TEST(ObjLoader, LoadMtl) {
  using sora::MemoryFile;
  string path1 = Filesystem::GetAppPath("mtl/example.mtl");
  MemoryFile file1(path1);
  EXPECT_EQ(false, file1.IsOpened());
  ASSERT_EQ(true, file1.Open());

  vector<Material> mtl_list;
  ObjLoader loader;
  loader.LoadMtl(file1.start, file1.end, &mtl_list);

/*
#sample material
newmtl flatwhite
Ka  0.5000  0.5000  0.5000
Kd  1.0000  1.0000  1.0000
illum 1

newmtl shinyred
Ka  0.1985  0.0000  0.0000
Kd  0.5921  0.0167  0.0000
Ks  0.5973  0.2083  0.2083
illum 2
Ns 100.2235

newmtl clearblue
Ka  0.0394  0.0394  0.3300
Kd  0.1420  0.1420  0.9500
illum 1
Tr 0.4300
*/

  ASSERT_EQ(3, mtl_list.size());
  Material &mtl1 = mtl_list[0];
  EXPECT_STREQ("flatwhite", mtl1.name.c_str());
  EXPECT_EQ(0.5f, mtl1.ambient[0]);
  EXPECT_EQ(0.5f, mtl1.ambient[1]);
  EXPECT_EQ(0.5f, mtl1.ambient[2]);
  EXPECT_EQ(1.0f, mtl1.diffuse[0]);
  EXPECT_EQ(1.0f, mtl1.diffuse[1]);
  EXPECT_EQ(1.0f, mtl1.diffuse[2]);
  //EXPECT_EQ(1, mtl1.illumination_model);


  Material &mtl2 = mtl_list[1];
  EXPECT_STREQ("shinyred", mtl2.name.c_str());
  EXPECT_EQ(0.1985f, mtl2.ambient[0]);
  EXPECT_EQ(0.0f, mtl2.ambient[1]);
  EXPECT_EQ(0.0f, mtl2.ambient[2]);
  EXPECT_EQ(0.5921f, mtl2.diffuse[0]);
  EXPECT_EQ(0.0167f, mtl2.diffuse[1]);
  EXPECT_EQ(0.0f, mtl2.diffuse[2]);
  EXPECT_EQ(0.5973f, mtl2.specular[0]);
  EXPECT_EQ(0.2083f, mtl2.specular[1]);
  EXPECT_EQ(0.2083f, mtl2.specular[2]);
  EXPECT_EQ(100.2235f, mtl2.shininess);
  //EXPECT_EQ(2, mtl2.illumination_model);

  Material &mtl3 = mtl_list[2];
  EXPECT_EQ(0.43f, mtl3.alpha);
}