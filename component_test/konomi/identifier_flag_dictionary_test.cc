// Ŭnicode please
#include "precompile.h"
#include "konomi/identifier_flag_dictionary.h"

using namespace std;
using namespace konomi;

TEST(IdentifierFlagDictionary, simpleUse)
{
	vector<string> strlist;
	strlist.push_back(string("asdf"));
	strlist.push_back(string("sample1"));
	strlist.push_back(string("demo2"));

	IdentifierFlagDictionary flagDict(strlist);
	
	EXPECT_EQ(0x01, flagDict.GetFlag(string("asdf")));
	EXPECT_EQ(0x02, flagDict.GetFlag(string("sample1")));
	EXPECT_EQ(0x04, flagDict.GetFlag(string("demo2")));
	EXPECT_EQ(0x00, flagDict.GetFlag(string("not exist")));

	EXPECT_EQ(string("asdf"), flagDict.GetIdentifier(0x01));
	EXPECT_EQ(string("sample1"), flagDict.GetIdentifier(0x02));
	EXPECT_EQ(string("demo2"), flagDict.GetIdentifier(0x04));
	EXPECT_EQ(string(), flagDict.GetIdentifier(0x05));
}

TEST(IdentifierFlagDictionary, GetIdentifiers)
{
	vector<string> strlist;
	strlist.push_back(string("asdf"));
	strlist.push_back(string("sample1"));
	strlist.push_back(string("demo2"));

	IdentifierFlagDictionary flagDict(strlist);

	vector<string> retval1;
	int result = flagDict.GetIdentifiers(0x5, retval1);
	EXPECT_EQ(2, result);
	//small flag first, big flag last
	EXPECT_EQ(string("asdf"), retval1[0]);
	EXPECT_EQ(string("demo2"), retval1[1]);

	list<string> retval2;
	result = flagDict.GetIdentifiers(0x8, retval2);
	EXPECT_EQ(0, result);
}