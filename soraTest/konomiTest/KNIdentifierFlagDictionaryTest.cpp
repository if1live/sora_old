// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "konomi/KNIdentifierFlagDictionary.h"

using namespace std;
using namespace konomi;

TEST(KNIdentifierFlagDictionary, simpleUse)
{
	vector<string> strlist;
	strlist.push_back(string("asdf"));
	strlist.push_back(string("sample1"));
	strlist.push_back(string("demo2"));

	IdentifierFlagDictionary flagDict(strlist);
	
	EXPECT_EQ(0x01, flagDict.getFlag(string("asdf")));
	EXPECT_EQ(0x02, flagDict.getFlag(string("sample1")));
	EXPECT_EQ(0x04, flagDict.getFlag(string("demo2")));
	EXPECT_EQ(0x00, flagDict.getFlag(string("not exist")));

	EXPECT_EQ(string("asdf"), flagDict.getIdentifier(0x01));
	EXPECT_EQ(string("sample1"), flagDict.getIdentifier(0x02));
	EXPECT_EQ(string("demo2"), flagDict.getIdentifier(0x04));
	EXPECT_EQ(string(), flagDict.getIdentifier(0x05));
}

TEST(KNIdentifierFlagDictionary, getIdentifiers)
{
	vector<string> strlist;
	strlist.push_back(string("asdf"));
	strlist.push_back(string("sample1"));
	strlist.push_back(string("demo2"));

	IdentifierFlagDictionary flagDict(strlist);

	vector<string> retval1;
	int result = flagDict.getIdentifiers(0x5, retval1);
	EXPECT_EQ(2, result);
	//small flag first, big flag last
	EXPECT_EQ(string("asdf"), retval1[0]);
	EXPECT_EQ(string("demo2"), retval1[1]);

	list<string> retval2;
	result = flagDict.getIdentifiers(0x8, retval2);
	EXPECT_EQ(0, result);
}