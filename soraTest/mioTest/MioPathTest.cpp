#include "../SoraTestStdAfx.h"

#include <mio/MioPath.h>
#include <mio/MioReader.h>

using namespace mio;
using std::string;

TEST(PathTest, appPath)
{
	//존재하지 않는 파일은 빈 문자열
	string p1 = Path::appPath("testdata/asd", "txt");
	string p2 = Path::appPath("testdata/asd.txt");
	string file = "testdata/asd.txt";
	string p3 = Path::appPath(file);
	EXPECT_EQ(p1, p2);
	EXPECT_EQ(p1, p3);
	EXPECT_STREQ("", p1.c_str());

	//존재하냐느 파일 기준으로 재확인
	p1 = Path::appPath("testdata/mio/books", "xml");
	p2 = Path::appPath("testdata/mio/books.xml");
	file = "testdata/mio/books.xml";
	p3 = Path::appPath(file);
	EXPECT_TRUE(p1.length() > 0);
	EXPECT_EQ(p1, p2);
	EXPECT_EQ(p1, p3);
}

TEST(PathTest, docPath)
{
	string p1 = Path::docPath("testdata/asd", "txt");
	string p2 = Path::docPath("testdata/asd.txt");
	string file = "testdata/asd.txt";
	string p3 = Path::docPath(file);
	EXPECT_EQ(p1, p2);
	EXPECT_EQ(p1, p3);

	//doc의 경우는 생성할수있으므로 파일이 없어도 경로대로 뜬다
	//엿으면 좋겠지만 아이폰의 경우는 샌드방스 정책떄문에 같지 않다
	//EXPECT_TRUE(p1 == file);
}

TEST(PathTest, isFileExist)
{
	//app path test
	string p1 = Path::appPath("testdata/mio/books.xml");
	EXPECT_EQ(true, Path::isFileExist(p1));
	
	string p2 = Path::appPath("testdata/mio/fdsjkl.xml");
	EXPECT_FALSE(Path::isFileExist(p2));
	
	string p3 = Path::docPath("testdata/mio/asd.txt");
	EXPECT_FALSE(Path::isFileExist(p3));
	
	//doc path에 임시파일 생성
	string p4 = Path::docPath("test.txt");
	FILE *f = fopen(p4.c_str(), "wb");
	fprintf(f, "test");
	fclose(f);
	EXPECT_EQ(true, Path::isFileExist(p4));
}
/*
TEST(File, filesize)
{
	string p1 = Path::appPath("testdata/books.xml");
	EXPECT_EQ(4406, File::filesize(p1));
}

TEST(File, copy)

{
	string p1 = Path::appPath("testdata/books.xml");
	string p2 = Path::docPath("copy_tmp");
	File::copy(p1, p2);
	EXPECT_EQ(File::filesize(p1), File::filesize(p2));
	
	//읽어서 내용 확인
	string content1 = mio::reader::read(p1);
	string content2 = mio::reader::read(p2);
	EXPECT_TRUE(content1 == content2);
}

*/
