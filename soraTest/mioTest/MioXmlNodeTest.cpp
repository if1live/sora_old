#include "../SoraTestStdAfx.h"

#include <mio/MioReader.h>
#include <mio/MioXmlNode.h>

using namespace std;
using namespace mio;

TEST(MioXmlNode, name)
{
	XmlNode n1("test1");
	EXPECT_STREQ(n1.getName().c_str(), "test1");

	string name("test2");
	XmlNode n2(name);
	EXPECT_TRUE(n2.getName() == name);
}

TEST(MioXmlNode, content)
{
	XmlNode n1("test1");
	n1.setContent("asdf");
	EXPECT_STREQ(n1.getContent().c_str(), "asdf");

	string str("test2");
	XmlNode n2("fds");
	n2.setContent(str);
	EXPECT_TRUE(n2.getContent() == str);	
}

TEST(XmlNodeTest, Constructor)
{
	XmlNode n("root");
	EXPECT_STREQ("root", n.getName().c_str());
	EXPECT_EQ(0, n.childSize());
}

TEST(XmlNodeTest, Attribute)
{
	XmlNode n("root");

	//attriute 넣기
	n.setAttribute("k1", "1");
	n.setAttribute("k2", "2");
	n.setAttribute("k2", "3");	//중복키->덮어쓰기

	const std::string &value1 = n.getAttribute("k1");
	EXPECT_STREQ("1", value1.c_str());

	const std::string &value2 = n.getAttribute("k2");
	EXPECT_STREQ("3", value2.c_str());

	//키가 존재하나 확인
	EXPECT_EQ(true, n.hasAttribute("k1"));
	EXPECT_FALSE(n.hasAttribute("none"));

	//attribute추가 설정
	n.setAttribute("id", "thisisroot");
	const std::string &idValue = n.getAttribute("id");
	EXPECT_STREQ("thisisroot", idValue.c_str());
}

TEST(XmlNodeTest, Children)
{
	XmlNode root("root");

	XmlNode *c1 = new XmlNode("child1");
	XmlNode *c2 = new XmlNode("child2");
	XmlNode *c3 = new XmlNode("child3");

	root.addChild(c1);
	root.addChild(c2);
	root.addChild(c3);

	EXPECT_EQ(3, root.childSize());

	XmlNodePtr first = root.firstChild();
	EXPECT_EQ(c1, first.get());

	XmlNodePtr last = root.lastChild();
	EXPECT_EQ(c3, last.get());

	//child get test
	XmlNodePtr c = root.child(1);
	EXPECT_EQ(c2, c.get());
	EXPECT_FALSE(c->isNull());

	//올바르지 않은 인덱스의 자식을 찾는 경우
	XmlNodePtr nullNode = NullXmlNode::null();
	XmlNodePtr notvalid = root.child(-1);
	EXPECT_EQ(true, notvalid->isNull());
	EXPECT_EQ(nullNode, notvalid);

	notvalid = root.child(3);
	EXPECT_EQ(true, notvalid->isNull());
	EXPECT_EQ(nullNode, notvalid);
}

TEST(NullXmlNodeTest, Test)
{
	XmlNodePtr nullNode = NullXmlNode::null();

	for(int i = -1 ; i < 3 ; i++)
	{
		EXPECT_EQ(nullNode->child(i), nullNode);
	}

	EXPECT_EQ(nullNode, nullNode->firstChild());
	EXPECT_EQ(nullNode, nullNode->lastChild());
	EXPECT_EQ(0, nullNode->childSize());
	EXPECT_EQ(nullNode.get(), nullNode->parent());
}

TEST(XmlNodeTest, getElementById)
{
	XmlNodePtr result;

	auto_ptr<XmlNode> root(new XmlNode("root"));
	root->setAttribute("id", "thisisroot");

	//자기자신의 id가 같은 경우, 근데 자기자신은 무시한다
	result = root->getElementById("thisisroot");
	EXPECT_EQ(result->isNull(), true);

	//자식중에서 id같은거 있는 경우
	XmlNode *child1 = new XmlNode("child");
	child1->setAttribute("id", "child1");
	root->addChild(child1);

	XmlNode *child2 = new XmlNode("child");
	root->addChild(child2);
	result = root->getElementById("child1");
	EXPECT_EQ(child1, result.get());

	//자식의 자식중에서 존재하는 경우
	XmlNode *child11 = new XmlNode("child_child");
	child11->setAttribute("id", "deep");
	child1->addChild(child11);

	result = root->getElementById("deep");
	EXPECT_EQ(child11, result.get());
}

TEST(XmlNodeTest, getElementByName)
{
	XmlNodeList result;

	auto_ptr<XmlNode> root(new XmlNode("root"));
	result = root->getElementsByName("root");
	EXPECT_EQ(0, result.size());	//자기 자신은 제외

	XmlNode *child1 = new XmlNode("child");
	root->addChild(child1);
	XmlNode *child2 = new XmlNode("child");
	root->addChild(child2);
	XmlNode *child3 = new XmlNode("child______");
	root->addChild(child3);

	result = root->getElementsByName("child");
	EXPECT_EQ(2, result.size());

	XmlNode *child11 = new XmlNode("child");
	child1->addChild(child11);
	XmlNode *child12 = new XmlNode("child__");
	child1->addChild(child12);

	result = root->getElementsByName("child");
	EXPECT_EQ(3, result.size());

	//자식1개
	result = child1->getElementsByName("child");
	EXPECT_EQ(1, result.size());
}

TEST(XmlNodeListTest, addSize)
{
	XmlNodeList list;
	EXPECT_EQ(0, list.size());

	list.add(new XmlNode("as"));
	EXPECT_EQ(1, list.size());

	for(int i = 0 ; i < 5 ; i++)
	{
		list.add(new XmlNode("fds"));
	}
	EXPECT_EQ(6, list.size());
}

TEST(XmlNodeListTest, basicMethod)
{
	XmlNodeList list;

	XmlNode *c1 = new XmlNode("child1");
	XmlNode *c2 = new XmlNode("child2");
	XmlNode *c3 = new XmlNode("child3");

	list.add(c1);
	list.add(c2);
	list.add(c3);

	EXPECT_EQ(3, list.size());

	XmlNodePtr first = list.get(0);
	EXPECT_EQ(c1, first.get());

	XmlNodePtr last = list.get(list.size()-1);
	EXPECT_EQ(c3, last.get());

	//child get test
	XmlNodePtr c = list.get(1);
	EXPECT_EQ(c2, c.get());
	EXPECT_FALSE(c->isNull());

	//올바르지 않은 인덱스의 자식을 찾는 경우
	XmlNodePtr nullNode = NullXmlNode::null();
	XmlNodePtr notvalid = list.get(-1);
	EXPECT_EQ(true, notvalid->isNull());
	EXPECT_EQ(nullNode, notvalid);

	notvalid = list.get(3);
	EXPECT_EQ(true, notvalid->isNull());
	EXPECT_EQ(nullNode, notvalid);
}
