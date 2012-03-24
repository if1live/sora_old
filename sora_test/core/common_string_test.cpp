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
#include "sora/core/common_string.h"

TEST(CommonString, Trim) {
  using std::string;
  using namespace sora;
	string a("as df");
	EXPECT_TRUE(Trim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(Trim(b) == "as df");
	string c("as df  ");
	EXPECT_TRUE(Trim(c) == "as df");
	string d("  as df  ");
	EXPECT_TRUE(Trim(d) == "as df");
}

TEST(CommonString, LeftTrim) {
  using std::string;
	string a("as df");
	EXPECT_TRUE(sora::LeftTrim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(sora::LeftTrim(b) == "as df");
	string c("as df  ");
	EXPECT_TRUE(sora::LeftTrim(c) == "as df  ");
	string d("  as df  ");
	EXPECT_TRUE(sora::LeftTrim(d) == "as df  ");
}

TEST(CommonString, RightTrim) {
  using std::string;
	string a("as df");
	EXPECT_TRUE(sora::RightTrim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(sora::RightTrim(b) == "  as df");
	string c("as df  ");
	EXPECT_TRUE(sora::RightTrim(c) == "as df");
	string d("  as df  ");
	EXPECT_TRUE(sora::RightTrim(d) == "  as df");
}


TEST(CommonString, Split) {
  using std::vector;
  using std::string;
	vector<string> result;
	int retval;
	
	//없는거 쪼개기 시도
	string test0("kldp.org");
	retval = sora::Split(test0, 'z', &result);
	EXPECT_EQ(1, retval);

	//1글자로 자르기
	string test1("kldp.org/node/11");
  result.clear();
	retval = sora::Split(test1, '/', &result);
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");

  
  /*
	//여러글자를 받아서 한개라도 등장하면 자르기
	//strtok의 delim같은 느낌?
	retval = sora::Split(test1, result, "node");
	EXPECT_EQ(4, retval);
	ASSERT_EQ(4, (int)result.size());
	EXPECT_TRUE(result[0] == "kl");
	EXPECT_TRUE(result[1] == "p.");
	EXPECT_TRUE(result[2] == "rg/");
	EXPECT_TRUE(result[3] == "/11");	
  */
}

TEST(sora, Join) {
  using std::string;
  using std::vector;

	string delim("::");
	string result;

	vector<string> strlist0;
	strlist0.push_back("a");
	strlist0.push_back("b");
	strlist0.push_back("c");
	result = sora::Join(delim, strlist0);
	EXPECT_TRUE(result == "a::b::c");
}

/*
TEST(sora, Tokenize)
{
	string test1("kldp.org/node/11");
	vector<String> result;
	int retval;

	///정상적으로 잘린 경우
	retval = sora::Tokenize(test1, result, "node");
	EXPECT_EQ(2, retval);
	ASSERT_EQ(2, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org/");
	EXPECT_TRUE(result[1] == "/11");

	//정상적으로 잘린경우 2
	retval = sora::Tokenize(test1, result, "/");
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");

	//글자는 있지만 문자열이 없어서 못자른 경우
	retval = sora::Tokenize(test1, result, "onde");
	EXPECT_EQ(1, retval);
	EXPECT_TRUE(result[0] == "kldp.org/node/11");

	//std::string같은 기본 클래스를 기반으로 뭔가를 수행하는 작업을 외부에서 더 많이 할거같다
}
*/