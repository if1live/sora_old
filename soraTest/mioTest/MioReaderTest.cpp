#include "../SoraTestStdAfx.h"
#include <mio/MioReader.h>
#include <mio/MioPath.h>

using namespace mio;
using std::vector;
using std::string;
using std::auto_ptr;

//#pragma mark file reader test
TEST(MioFileReaderTest, read)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	EXPECT_TRUE(file != "");

	FileReader reader;
	string result = reader.read(file);
	string expected("this is line1.\nthis is line2.\n");
	EXPECT_EQ(result, expected);
}

TEST(MioFileReaderTest, readline)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");

	FileReader reader;
	vector<string> result = reader.readline(file);
	
	ASSERT_EQ(2, result.size());
	EXPECT_STREQ("this is line1.", result[0].c_str());
	EXPECT_STREQ("this is line2.", result[1].c_str());
}

TEST(BufferFileReaderTest, readChar)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	BufferFileReader reader(file, 2);	//의도적으로 작은버퍼 생성후 테스트

	const char *expected = "this is line1.\nthis is line2.\n";
	for(int i = 0 ; i < sizeof(expected) / sizeof(char) ; i++)
	{
		unsigned char ch = reader.readChar();
		unsigned char exCh = expected[i];
		EXPECT_EQ(ch, exCh);
	}
}
TEST(BufferFileReaderTest, filesize)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	int expected = 30;

	BufferFileReader reader(file, 2);	//의도적으로 작은버퍼 생성후 테스트
	int actual = reader.filesize();
	EXPECT_EQ(expected, actual);
}
TEST(BufferFileReaderTest, canRead)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	BufferFileReader reader(file, 2);	//의도적으로 작은버퍼 생성후 테스트
	for(int i = 0 ; i < reader.filesize() ; i++)
	{
		bool read = reader.canRead();
		EXPECT_EQ(true, read);
		reader.readChar();
	}
	//읽기 종료
	for(int i = 0 ; i < 3 ; i++)
	{
		bool read = reader.canRead();
		EXPECT_TRUE(false == read);
		unsigned char ch = reader.readChar();
		EXPECT_TRUE('\0' == ch);
	}
}
TEST(BufferFileReaderTest, canRead1)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	BufferFileReader reader(file, 3);	//의도적으로 작은버퍼 생성후 테스트
	for(int i = 0 ; i < reader.filesize() ; i++)
	{
		bool read = reader.canRead();
		EXPECT_EQ(true, read);
		reader.readChar();
	}
	//읽기 종료
	for(int i = 0 ; i < 3 ; i++)
	{
		bool read = reader.canRead();
		EXPECT_TRUE(false == read);
		unsigned char ch = reader.readChar();
		EXPECT_TRUE('\0' == ch);
	}
}
TEST(BufferFileReaderTest, readLine)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	BufferFileReader reader(file, 2);	//의도적으로 작은버퍼 생성후 테스트
	//read line 1
	EXPECT_EQ(true, reader.canRead());
	string line1 = reader.readLine();
	EXPECT_STREQ("this is line1.", line1.c_str());

	//read line2
	EXPECT_EQ(true, reader.canRead());
	string line2 = reader.readLine();
	EXPECT_STREQ("this is line2.", line2.c_str());

	EXPECT_FALSE(reader.canRead());
}
TEST(BufferFileReaderTest, readWord)
{
	string file = Path::appPath("testdata/mio/readerTest.txt");
	BufferFileReader reader(file, 2);	//의도적으로 작은버퍼 생성후 테스트
	string word;

	EXPECT_EQ(true, reader.canRead());
	word = reader.readWord();
	EXPECT_STREQ("this", word.c_str());

	EXPECT_EQ(true, reader.canRead());
	word = reader.readWord();
	EXPECT_STREQ("is", word.c_str());

	EXPECT_EQ(true, reader.canRead());
	word = reader.readWord();
	EXPECT_STREQ("line1.", word.c_str());

	EXPECT_EQ(true, reader.canRead());
	word = reader.readWord();
	EXPECT_STREQ("this", word.c_str());

	EXPECT_EQ(true, reader.canRead());
	word = reader.readWord();
	EXPECT_STREQ("is", word.c_str());

	EXPECT_EQ(true, reader.canRead());
	word = reader.readWord();
	EXPECT_STREQ("line2.", word.c_str());

	EXPECT_FALSE(reader.canRead());
}
