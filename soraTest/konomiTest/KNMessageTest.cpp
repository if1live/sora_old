// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "konomi/KNMessage.h"

using namespace std;
using namespace konomi;

TEST(KNMessage, test)
{
	Message<8> msg(0);
	EXPECT_EQ(0, msg.getCode());
	EXPECT_EQ(0, msg.getNumParam());

	//param test
	int a = 1;
	msg.addParam(Variant(1));
	msg.addParam(Variant(2.0f));
	msg.addParam(Variant(true));
	msg.addParam(Variant(&a));

	EXPECT_EQ(4, msg.getNumParam());

	const Variant &param1 = msg.getParam(0);
	EXPECT_EQ(VariantTypeInt, param1.getType());
	EXPECT_EQ(1, (int)param1);

	const Variant &param2 = msg.getParam(1);
	EXPECT_EQ(VariantTypeFloat, param2.getType());
	EXPECT_EQ(2.0f, (float)param2);

	const Variant &param3 = msg.getParam(2);
	EXPECT_EQ(VariantTypeBool, param3.getType());
	EXPECT_EQ(true, (bool)param3);

	const Variant &param4 = msg.getParam(3);
	EXPECT_EQ(VariantTypePointer, param4.getType());
	EXPECT_EQ(&a, param4.getAsPointer<int>());
}

TEST(KNMessage, op)
{
	Message<2> msg1(1);
	msg1.addParam(Variant(1));
	msg1.addParam(Variant(2.0f));

	Message<4> msg2(1);
	msg2.addParam(Variant(1));
	msg2.addParam(Variant(2.0f));

	Message<3> msg3(3);
	msg3.addParam(Variant(true));

	EXPECT_EQ(true, msg1 == msg2);
	EXPECT_EQ(false, msg1 != msg2);

	EXPECT_EQ(true, msg1 != msg3);
	EXPECT_EQ(false, msg1 == msg3);
}

TEST(KNMessage, paramCount_up_down)
{
	Message<4> msg1(1);
	msg1.addParam(Variant(1));
	msg1.addParam(Variant(2));

	//인자수 내리기
	Message<2> msg2(msg1);
	EXPECT_EQ(true, msg2 == msg1);

	//인자수 올리기
	Message<6> msg3(msg1);
	EXPECT_EQ(true, msg3 == msg1);
}

TEST(KNMessage, canAddParam)
{
	Message<2> msg(1);
	EXPECT_EQ(true, msg.canAddParam());

	msg.addParam(Variant(1));
	EXPECT_EQ(true, msg.canAddParam());

	msg.addParam(Variant(1));
	EXPECT_EQ(false, msg.canAddParam());
}

TEST(KNMessage, setNumParam)
{
	Message<4> msg(1);
	EXPECT_EQ(0, msg.getNumParam());

	msg.setNumParam(2);
	for(int i = 0 ; i < msg.MaxVariantNumber ; i++)
	{
		const Variant &v = msg.getParam(i);
		if(i < 2)
		{
			EXPECT_EQ(false, v.isNull());
		}
		else
		{
			EXPECT_EQ(true, v.isNull());
		}
	}
}

TEST(KNMessage, setParam)
{
	Message<4> msg(1);
	msg.setNumParam(2);
	msg.setParam(1, Variant(1.0f));
	
	EXPECT_EQ(VariantTypeInt, msg.getParam(0).getType());
	EXPECT_EQ(VariantTypeFloat, msg.getParam(1).getType());
	EXPECT_EQ(VariantTypeNull, msg.getParam(2).getType());
	EXPECT_EQ(VariantTypeNull, msg.getParam(3).getType());
}