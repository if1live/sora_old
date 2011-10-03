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

#include "mio/path.h"

using mio::Path;
using std::string;

TEST(PathTest, AppPath) {
  // 존재하지 않는 파일은 빈 문자열
  string p1 = Path::AppPath("testdata/asd", "txt");
  string p2 = Path::AppPath("testdata/asd.txt");
  string file = "testdata/asd.txt";
  string p3 = Path::AppPath(file);
  EXPECT_EQ(p1, p2);
  EXPECT_EQ(p1, p3);
  EXPECT_STREQ("", p1.c_str());

  // 존재하냐느 파일 기준으로 재확인
  p1 = Path::AppPath("testdata/mio/books", "xml");
  p2 = Path::AppPath("testdata/mio/books.xml");
  file = "testdata/mio/books.xml";
  p3 = Path::AppPath(file);
  EXPECT_GT(p1.length(), 0);
  EXPECT_EQ(p1, p2);
  EXPECT_EQ(p1, p3);
}

TEST(PathTest, DocPath) {
  string p1 = Path::DocPath("testdata/asd", "txt");
  string p2 = Path::DocPath("testdata/asd.txt");
  string file = "testdata/asd.txt";
  string p3 = Path::DocPath(file);
  EXPECT_EQ(p1, p2);
  EXPECT_EQ(p1, p3);

  // doc의 경우는 생성할수있으므로 파일이 없어도 경로대로 뜬다
  // 엿으면 좋겠지만 아이폰의 경우는 샌드방스 정책떄문에 같지 않다
  // EXPECT_TRUE(p1 == file);
}

TEST(PathTest, IsFileExist) {
  // app path test
  string p1 = Path::AppPath("testdata/mio/books.xml");
  EXPECT_EQ(true, Path::IsFileExist(p1));

  string p2 = Path::AppPath("testdata/mio/fdsjkl.xml");
  EXPECT_FALSE(Path::IsFileExist(p2));

  string p3 = Path::DocPath("testdata/mio/asd.txt");
  EXPECT_FALSE(Path::IsFileExist(p3));

  // doc path에 임시파일 생성
  string p4 = Path::DocPath("test.txt");
  FILE *f = fopen(p4.c_str(), "wb");
  fprintf(f, "test");
  fclose(f);
  EXPECT_EQ(true, Path::IsFileExist(p4));
}
