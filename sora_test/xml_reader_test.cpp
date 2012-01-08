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
#include "sora/xml_reader.h"
#include "sora/xml_node.h"
#include "sora/memory_file.h"

using namespace sora;
using namespace std;


TEST(XmlReaderTest, test) {
  const char *path1 = "testdata/books.xml";
  MemoryFile file1(path1);
  file1.Open();
  const unsigned char *content = file1.start;

	TiXmlDocument doc;
}


TEST(XmlReaderTest, read) {
	const char *path1 = "testdata/books.xml";
  MemoryFile file1(path1);
  file1.Open();
  const unsigned char *content = file1.start;

	XmlReader xmlreader;
  XmlNode root;
	
  bool read_result = xmlreader.Read(&root, content);

	//error test
	EXPECT_EQ(true, read_result);
	EXPECT_EQ(false, xmlreader.IsErrorOccur());
	EXPECT_TRUE(NULL ==xmlreader.GetError());

	//root의 정보 확인
	EXPECT_STREQ("catalog", root.name().c_str());
	EXPECT_EQ(12, root.ChildCount());
	
	//자식은 생긴게 똑같은형태라서 1개만 확인함
	XmlNode *child = root.Child(0);
	EXPECT_STREQ("book", child->name().c_str());
	EXPECT_EQ(6, child->ChildCount());
	EXPECT_STREQ("bk101", child->GetAttribute("id").c_str());

	//자식 1개씩 확인
	XmlNode *inner = child->Child(0);
	EXPECT_STREQ("author", inner->name().c_str());
	EXPECT_STREQ("Gambardella, Matthew", inner->content().c_str());

	inner = child->Child(5);
	EXPECT_STREQ("description", inner->name().c_str());
	EXPECT_STREQ("An in-depth look at creating applications with XML.", inner->content().c_str());
}


TEST(XmlReaderTest, read2) {
  const char *path1 = "testdata/monster.xml";
  MemoryFile file1(path1);
  file1.Open();
  const unsigned char *content = file1.start;

  XmlReader xmlreader;
  XmlNode root;
	bool read_result = xmlreader.Read(&root, content);
  EXPECT_EQ(true, read_result);

  /*
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
  */
}

//문법에러있는 xml 읽기
TEST(XmlReaderTest, error_read) {
	string xml = "<test><test>";
	XmlReader reader;
  XmlNode node;
	bool read_result = reader.Read(&node, xml);
	EXPECT_EQ(false, read_result);
	EXPECT_EQ(true, reader.IsErrorOccur());
	EXPECT_TRUE(NULL != reader.GetError());
}
