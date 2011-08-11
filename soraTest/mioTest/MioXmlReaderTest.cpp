#include "../SoraTestStdAfx.h"
#include <tinyxml.h>
#include <mio/MioPath.h>
#include <mio/MioReader.h>
#include <mio/MioXmlReader.h>


using namespace mio;
using namespace std;

TEST(MioXmlReaderTest, test)
{
	string file = Path::appPath("testdata/mio/books.xml");
	FileReader reader;
	string content = reader.read(file);
	TiXmlDocument doc;
	doc.Parse(content.c_str());
}


TEST(MioXmlReaderTest, read)
{
	
	string file = Path::appPath("testdata/mio/books.xml");
	FileReader reader;
	string content = reader.read(file);

	XmlReader xmlreader;
	XmlNodePtr root = xmlreader.read(content);
	//error test
	EXPECT_EQ(false, root->isNull());
	EXPECT_EQ(false, xmlreader.isErrorOccur());
	EXPECT_TRUE(NULL ==xmlreader.getError());
	EXPECT_TRUE(root.get() != NULL);

	//root의 정보 확인
	EXPECT_STREQ("catalog", root->getName().c_str());
	EXPECT_EQ(12, root->childSize());
	
	//자식은 생긴게 똑같은형태라서 1개만 확인함
	XmlNodePtr child = root->child(0);
	EXPECT_STREQ("book", child->getName().c_str());
	EXPECT_EQ(6, child->childSize());
	EXPECT_STREQ("bk101", child->getAttribute("id").c_str());

	//자식 1개씩 확인
	XmlNodePtr inner = child->child(0);
	EXPECT_STREQ("author", inner->getName().c_str());
	EXPECT_STREQ("Gambardella, Matthew", inner->getContent().c_str());

	inner = child->child(5);
	EXPECT_STREQ("description", inner->getName().c_str());
	EXPECT_STREQ("An in-depth look at creating applications with XML.", inner->getContent().c_str());
}


TEST(MioXmlReaderTest, read2)
{
	string file = Path::appPath("testdata/mio/monster.xml");
	mio::FileReader reader;
	string content = reader.read(file);

	mio::XmlReader xmlreader;
	mio::XmlNodePtr root = xmlreader.read(content);
	EXPECT_EQ(false, root->isNull());

	mio::XmlNodeList monsterNodeList = root->getElementsByName("monster");
	mio::XmlNodePtrIter iter = monsterNodeList.begin();
	for( ; iter != monsterNodeList.end() ; iter++)
	{
		mio::XmlNodePtr nodePtr = *iter;
		mio::XmlNodePtrIter childIter;
		for(childIter = nodePtr->childBegin() ; childIter != nodePtr->childEnd() ; childIter++)
		{
			mio::XmlNodePtr attrNode = *childIter;
			const std::string &key = attrNode->getAttribute("key");
			const std::string &value = attrNode->getAttribute("value");
			EXPECT_TRUE(key.length() > 0);
			EXPECT_TRUE(value.length() > 0);
		}
	}
}

//문법에러있는 xml 읽기
TEST(MioXmlReaderTest, error_read)
{
	string xml = "<test><test>";
	XmlReader reader;
	XmlNodePtr node = reader.read(xml);
	EXPECT_EQ(true, node->isNull());
	EXPECT_EQ(true, reader.isErrorOccur());
	EXPECT_TRUE(NULL != reader.getError());
}
