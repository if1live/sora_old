#include "../SoraTestStdAfx.h"

#include "matsu/MTStringUtil.h"

using namespace matsu;
using namespace std;

TEST(MTStringUtil, trim)
{
	String a("as df");
	EXPECT_TRUE(StringUtil::trim(a) == "as df");
	String b("  as df");
	EXPECT_TRUE(StringUtil::trim(b) == "as df");
	String c("as df  ");
	EXPECT_TRUE(StringUtil::trim(c) == "as df");
	String d("  as df  ");
	EXPECT_TRUE(StringUtil::trim(d) == "as df");
}

TEST(MTStringUtil, leftTrim)
{
	String a("as df");
	EXPECT_TRUE(StringUtil::leftTrim(a) == "as df");
	String b("  as df");
	EXPECT_TRUE(StringUtil::leftTrim(b) == "as df");
	String c("as df  ");
	EXPECT_TRUE(StringUtil::leftTrim(c) == "as df  ");
	String d("  as df  ");
	EXPECT_TRUE(StringUtil::leftTrim(d) == "as df  ");
}

TEST(MTStringUtil, rightTrim)
{
	String a("as df");
	EXPECT_TRUE(StringUtil::rightTrim(a) == "as df");
	String b("  as df");
	EXPECT_TRUE(StringUtil::rightTrim(b) == "  as df");
	String c("as df  ");
	EXPECT_TRUE(StringUtil::rightTrim(c) == "as df");
	String d("  as df  ");
	EXPECT_TRUE(StringUtil::rightTrim(d) == "  as df");
}

TEST(MTStringUtil, split)
{
	vector<String> result;
	int retval;
	
	//없는거 쪼개기 시도
	String test0("kldp.org");
	retval = StringUtil::split(test0, result, "");
	EXPECT_EQ(-1, retval);

	//1글자로 자르기
	String test1("kldp.org/node/11");
	retval = StringUtil::split(test1, result, '/');
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");

	//여러글자를 받아서 한개라도 등장하면 자르기
	//strtok의 delim같은 느낌?
	retval = StringUtil::split(test1, result, "node");
	EXPECT_EQ(4, retval);
	ASSERT_EQ(4, (int)result.size());
	EXPECT_TRUE(result[0] == "kl");
	EXPECT_TRUE(result[1] == "p.");
	EXPECT_TRUE(result[2] == "rg/");
	EXPECT_TRUE(result[3] == "/11");	
}

TEST(MTStringUtil, tokenize)
{
	String test1("kldp.org/node/11");
	vector<String> result;
	i32 retval;

	///정상적으로 잘린 경우
	retval = StringUtil::tokenize(test1, result, "node");
	EXPECT_EQ(2, retval);
	ASSERT_EQ(2, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org/");
	EXPECT_TRUE(result[1] == "/11");

	//정상적으로 잘린경우 2
	retval = StringUtil::tokenize(test1, result, "/");
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");

	//글자는 있지만 문자열이 없어서 못자른 경우
	retval = StringUtil::tokenize(test1, result, "onde");
	EXPECT_EQ(1, retval);
	EXPECT_TRUE(result[0] == "kldp.org/node/11");

	//std::string같은 기본 클래스를 기반으로 뭔가를 수행하는 작업을 외부에서 더 많이 할거같다
}


TEST(StringUtil, join)
{
	String delim(":");
	String result;

	vector<string> strlist0;
	strlist0.push_back("a");
	strlist0.push_back("b");
	strlist0.push_back("c");
	result = StringUtil::join(delim, strlist0);
	EXPECT_TRUE(result == "a:b:c");

	vector<String> strlist1;
	strlist1.push_back(String("a"));
	strlist1.push_back(String("b"));
	strlist1.push_back(String("c"));
	result = StringUtil::join(delim, strlist1);
	EXPECT_TRUE(result == "a:b:c");

}
