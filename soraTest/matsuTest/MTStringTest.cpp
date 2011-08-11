#include "../SoraTestStdAfx.h"

#include "matsu/MTString.h"

using namespace matsu;
using namespace std;

TEST(MTString, construct)
{
	//string version
	String a("Test");
	EXPECT_TRUE(string("Test") == a());
	String b(string("Test"));
	EXPECT_TRUE(string("Test") == b());
	String c(b);
	EXPECT_TRUE(string("Test") == c());
	
	//wstring version
	WString wa(L"Test");
	EXPECT_TRUE(wstring(L"Test") == wa());
	WString wb(wstring(L"Test"));
	EXPECT_TRUE(wstring(L"Test") == wb());
	WString wc(wb);
	EXPECT_TRUE(wstring(L"Test") == wc());
}

TEST(MTString, assignment)
{
	String a("a");
	a = "char";
	EXPECT_TRUE(a == "char");
	a = string("string");
	EXPECT_TRUE(a == "string");
	a = String("String");
	EXPECT_TRUE(a == "String");
}

TEST(MTString, operator_eq)
{
	String a("Test");
	String b("Test");
	String c("test");

	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a != c);
	EXPECT_TRUE(a == "Test");
	EXPECT_TRUE(a != "test");
	EXPECT_TRUE(a == string("Test"));
	EXPECT_TRUE(a != string("test"));
}

TEST(MTSTring, operator_add)
{
	String a("a");
	a = a + "b";
	EXPECT_TRUE(a == "ab");
	a = a + string("c");
	EXPECT_TRUE(a == "abc");
	a = a + String("d");
	EXPECT_TRUE(a == "abcd");
	
	String b("a");
	b += "b";
	EXPECT_TRUE(b == "ab");
	b += string("c");
	EXPECT_TRUE(b == "abc");
	b += String("d");
	EXPECT_TRUE(b == "abcd");
	
	//한글도 해보기
	String c("한");
	c += "글";
	EXPECT_TRUE(c == "한글");
	
	//wstring
	WString d(L"한");
	d += L"글";
	EXPECT_TRUE(d == L"한글");
}

TEST(String, length)
{
	WString a(L"abcd");
	EXPECT_EQ(4, (int)a.length());
	
	WString b;
	EXPECT_EQ(0, (int)b.length());
}

TEST(String, array)
{
	String a("abc");
	EXPECT_EQ('a', a[0]);
}
