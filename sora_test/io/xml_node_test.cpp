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
#include "sora/xml_node.h"

using namespace sora;
using namespace std;

TEST(XmlNode, name) {
	XmlNode n1("test1");
	EXPECT_STREQ(n1.name().c_str(), "test1");

	string name("test2");
	XmlNode n2(name);
	EXPECT_TRUE(n2.name() == name);
}

TEST(XmlNode, content) {
	XmlNode n1("test1");
	n1.set_content("asdf");
	EXPECT_STREQ(n1.content().c_str(), "asdf");

	string str("test2");
	XmlNode n2("fds");
	n2.set_content(str);
	EXPECT_TRUE(n2.content() == str);	
}

TEST(NodeTest, Constructor) {
	XmlNode n("root");
	EXPECT_STREQ("root", n.name().c_str());
	EXPECT_EQ(0, n.ChildCount());
}

TEST(XmlNodeTest, Attribute) {
	XmlNode n("root");

	//attriute 넣기
	n.SetAttribute("k1", "1");
	n.SetAttribute("k2", "2");
	n.SetAttribute("k2", "3");	//중복키->덮어쓰기

	const std::string &value1 = n.GetAttribute("k1");
	EXPECT_STREQ("1", value1.c_str());

	const std::string &value2 = n.GetAttribute("k2");
	EXPECT_STREQ("3", value2.c_str());

	//키가 존재하나 확인
	EXPECT_EQ(true, n.HasAttribute("k1"));
	EXPECT_FALSE(n.HasAttribute("none"));

	//attribute추가 설정
	n.SetAttribute("id", "thisisroot");
	const std::string &idValue = n.GetAttribute("id");
	EXPECT_STREQ("thisisroot", idValue.c_str());
}

TEST(XmlNodeTest, Children) {
	XmlNode root("root");

	XmlNode *c1 = new XmlNode("child1");
	XmlNode *c2 = new XmlNode("child2");
	XmlNode *c3 = new XmlNode("child3");

	root.AddChild(c1);
	root.AddChild(c2);
	root.AddChild(c3);

	EXPECT_EQ(3, root.ChildCount());

	XmlNode *first = root.FirstChild();
	EXPECT_EQ(c1, first);

	XmlNode *last = root.LastChild();
	EXPECT_EQ(c3, last);

	//child get test
	XmlNode *c = root.Child(1);
	EXPECT_EQ(c2, c);

	//올바르지 않은 인덱스의 자식을 찾는 경우
	XmlNode *notvalid = root.Child(-1);
	EXPECT_EQ(NULL, notvalid);

	notvalid = root.Child(3);
	EXPECT_EQ(NULL, notvalid);
}
