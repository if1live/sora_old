#include "precompile.h"
#include "sora/string_util.h"

using namespace sora;
using namespace std;

TEST(StringUtil, Trim)
{
	string a("as df");
	EXPECT_TRUE(StringUtil::Trim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(StringUtil::Trim(b) == "as df");
	string c("as df  ");
	EXPECT_TRUE(StringUtil::Trim(c) == "as df");
	string d("  as df  ");
	EXPECT_TRUE(StringUtil::Trim(d) == "as df");
}

TEST(StringUtil, LeftTrim)
{
	string a("as df");
	EXPECT_TRUE(StringUtil::LeftTrim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(StringUtil::LeftTrim(b) == "as df");
	string c("as df  ");
	EXPECT_TRUE(StringUtil::LeftTrim(c) == "as df  ");
	string d("  as df  ");
	EXPECT_TRUE(StringUtil::LeftTrim(d) == "as df  ");
}

TEST(StringUtil, RightTrim)
{
	string a("as df");
	EXPECT_TRUE(StringUtil::RightTrim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(StringUtil::RightTrim(b) == "  as df");
	string c("as df  ");
	EXPECT_TRUE(StringUtil::RightTrim(c) == "as df");
	string d("  as df  ");
	EXPECT_TRUE(StringUtil::RightTrim(d) == "  as df");
}


TEST(StringUtil, Split)
{
	vector<string> result;
	int retval;
	
	//���°� �ɰ��� �õ�
	string test0("kldp.org");
	retval = StringUtil::Split(test0, 'z', &result);
	EXPECT_EQ(1, retval);

	//1���ڷ� �ڸ���
	string test1("kldp.org/node/11");
  result.clear();
	retval = StringUtil::Split(test1, '/', &result);
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");

  
  /*
	//�������ڸ� �޾Ƽ� �Ѱ��� �����ϸ� �ڸ���
	//strtok�� delim���� ����?
	retval = StringUtil::Split(test1, result, "node");
	EXPECT_EQ(4, retval);
	ASSERT_EQ(4, (int)result.size());
	EXPECT_TRUE(result[0] == "kl");
	EXPECT_TRUE(result[1] == "p.");
	EXPECT_TRUE(result[2] == "rg/");
	EXPECT_TRUE(result[3] == "/11");	
  */
}

TEST(StringUtil, Join)
{
	string delim("::");
	string result;

	vector<string> strlist0;
	strlist0.push_back("a");
	strlist0.push_back("b");
	strlist0.push_back("c");
	result = StringUtil::Join(delim, strlist0);
	EXPECT_TRUE(result == "a::b::c");
}

/*
TEST(StringUtil, Tokenize)
{
	string test1("kldp.org/node/11");
	vector<String> result;
	int retval;

	///���������� �߸� ���
	retval = StringUtil::Tokenize(test1, result, "node");
	EXPECT_EQ(2, retval);
	ASSERT_EQ(2, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org/");
	EXPECT_TRUE(result[1] == "/11");

	//���������� �߸���� 2
	retval = StringUtil::Tokenize(test1, result, "/");
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");

	//���ڴ� ������ ���ڿ��� ��� ���ڸ� ���
	retval = StringUtil::Tokenize(test1, result, "onde");
	EXPECT_EQ(1, retval);
	EXPECT_TRUE(result[0] == "kldp.org/node/11");

	//std::string���� �⺻ Ŭ������ ������� ������ �����ϴ� �۾��� �ܺο��� �� ���� �ҰŰ���
}
*/