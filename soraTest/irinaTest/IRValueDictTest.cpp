#include "../SoraTestStdAfx.h"

#include <irina/IRValueDict.h>

using namespace std;
using namespace matsu;
using namespace irina;

TEST(IRAbstractValue, getType)
{
	int a = 1;
	ValueContainer<int> va(a);
	EXPECT_EQ(ValueTypeContainer, va.getType());

	ValueDict dict;
	EXPECT_EQ(ValueTypeDict, dict.getType());
}

TEST(IRAbstractValue, get)
{
	int a = 1;
	float b = 2.0f;
	string c = "a";
	AbstractValue *va = new ValueContainer<int>(a);
	AbstractValue *vb = new ValueContainer<float>(b);
	AbstractValue *vc = new ValueContainer<string>(c);

	EXPECT_EQ(a, va->get<int>());
	EXPECT_EQ(b, vb->get<float>());
	EXPECT_EQ(c, vc->get<string>());
}

TEST(IRValueContainer, container)
{
	int a = 1;
	float b = 2.0f;
	string c = "a";
	ValueContainer<int> va(a);
	ValueContainer<float> vb(b);
	ValueContainer<string> vc(c);

	EXPECT_EQ(a, va.get());
	EXPECT_EQ(b, vb.get());
	EXPECT_EQ(c, vc.get());
}

TEST(IRValueDict, setGetContainer)
{
	int a = 1;
	float b = 2.0f;
	string c = "a";
	ValueContainer<int> *va = new ValueContainer<int>(a);
	ValueContainer<float> *vb = new ValueContainer<float>(b);
	ValueContainer<string> *vc = new ValueContainer<string>(c);

	ValueDict dict;
	dict.set("1", va);
	dict.set("2", vb);
	dict.set("3", vc);

	EXPECT_EQ(a, dict.get<int>("1"));
	EXPECT_EQ(b, dict.get<float>("2"));
	EXPECT_EQ(c, dict.get<string>("3"));

	//without typename은 불가능하더라 
	//return값의 T를 얻는것은 템플릿 구조상 불가능한듯하다. 위의 3줄 지원으로 만족하자
	//const int& aa = dict.getContainer("1");
}

TEST(IRValueDict, add)
{
	string s = "a";

	ValueDict dict;
	dict.add("1", static_cast<int>(1));
	dict.add("2", static_cast<float>(2.0));
	dict.add("3", static_cast<string>(s));

	EXPECT_EQ(1, dict.get<int>("1"));
	EXPECT_EQ(2.0f, dict.get<float>("2"));
	EXPECT_STREQ(dict.get<string>("3").c_str(), "a");

}

TEST(IRValueDict, get)
{
	int a = 1;
	ValueContainer<int> *va = new ValueContainer<int>(a);
	ValueDict *vb = new ValueDict();

	ValueDict dict;
	dict.set("1", va);
	dict.set("2", vb);

	//get
	AbstractValue *value = NULL;
	value = dict.get("1");
	EXPECT_EQ(ValueTypeContainer, value->getType());
	EXPECT_TRUE(va == static_cast< ValueContainer<int>*>(value));

	value = dict.get("2");
	EXPECT_EQ(ValueTypeDict, value->getType());
	EXPECT_TRUE(vb == static_cast<ValueDict*>(value));

	//존재하지 않는거 얻기 시도
	value = dict.get("not exist");
	EXPECT_EQ(ValueTypeContainer, value->getType());
	EXPECT_TRUE(value->isNull());

	//use []
	value = dict["1"];
	EXPECT_EQ(ValueTypeContainer, value->getType());
	EXPECT_TRUE(va == static_cast<ValueContainer<int>*>(value));

	value = dict["2"];
	EXPECT_EQ(ValueTypeDict, value->getType());
	EXPECT_TRUE(vb == static_cast<ValueDict*>(value));

	//존재하지 않는거 얻기 시도
	value = dict["not exist"];
	EXPECT_EQ(ValueTypeContainer, value->getType());
	EXPECT_TRUE(value->isNull());
}

TEST(ValueDict, exist)
{
	int a = 1;
	ValueContainer<int> *va = new ValueContainer<int>(a);
	ValueDict dict;
	dict.set("1", va);

	EXPECT_TRUE(dict.exist("1"));
	EXPECT_FALSE(dict.exist("fdsfds"));
}
