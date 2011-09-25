// Ŭnicode please
#include "precompile.h"
#include "konomi/message.h"

using namespace std;
using namespace konomi;

TEST(Message, test)
{
	Message<8> msg(0);
	EXPECT_EQ(0, msg.get_code());
	EXPECT_EQ(0, msg.get_num_param());

	//param test
	int a = 1;
	msg.AddParam(Variant(1));
	msg.AddParam(Variant(2.0f));
	msg.AddParam(Variant(true));
	msg.AddParam(Variant(&a));

	EXPECT_EQ(4, msg.get_num_param());

	const Variant &param1 = msg.GetParam(0);
	EXPECT_EQ(kVariantTypeInt, param1.get_type());
	EXPECT_EQ(1, (int)param1);

	const Variant &param2 = msg.GetParam(1);
	EXPECT_EQ(kVariantTypeFloat, param2.get_type());
	EXPECT_EQ(2.0f, (float)param2);

	const Variant &param3 = msg.GetParam(2);
	EXPECT_EQ(kVariantTypeBool, param3.get_type());
	EXPECT_EQ(true, (bool)param3);

	const Variant &param4 = msg.GetParam(3);
	EXPECT_EQ(kVariantTypePointer, param4.get_type());
	EXPECT_EQ(&a, param4.GetAsPointer<int>());
}

TEST(Message, op)
{
	Message<2> msg1(1);
	msg1.AddParam(Variant(1));
	msg1.AddParam(Variant(2.0f));

	Message<4> msg2(1);
	msg2.AddParam(Variant(1));
	msg2.AddParam(Variant(2.0f));

	Message<3> msg3(3);
	msg3.AddParam(Variant(true));

	EXPECT_EQ(true, msg1 == msg2);
	EXPECT_EQ(false, msg1 != msg2);

	EXPECT_EQ(true, msg1 != msg3);
	EXPECT_EQ(false, msg1 == msg3);
}

TEST(Message, paramCount_up_down)
{
	Message<4> msg1(1);
	msg1.AddParam(Variant(1));
	msg1.AddParam(Variant(2));

	//인자수 내리기
	Message<2> msg2(msg1);
	EXPECT_EQ(true, msg2 == msg1);

	//인자수 올리기
	Message<6> msg3(msg1);
	EXPECT_EQ(true, msg3 == msg1);
}

TEST(Message, CanAddParam)
{
	Message<2> msg(1);
	EXPECT_EQ(true, msg.CanAddParam());

	msg.AddParam(Variant(1));
	EXPECT_EQ(true, msg.CanAddParam());

	msg.AddParam(Variant(1));
	EXPECT_EQ(false, msg.CanAddParam());
}

TEST(Message, SetNumParam)
{
	Message<4> msg(1);
	EXPECT_EQ(0, msg.get_num_param());

	msg.SetNumParam(2);
	for(int i = 0 ; i < msg.MaxVariantNumber ; i++)
	{
		const Variant &v = msg.GetParam(i);
		if(i < 2)
		{
			EXPECT_EQ(false, v.IsNull());
		}
		else
		{
			EXPECT_EQ(true, v.IsNull());
		}
	}
}

TEST(Message, SetParam)
{
	Message<4> msg(1);
	msg.SetNumParam(2);
	msg.SetParam(1, Variant(1.0f));
	
	EXPECT_EQ(kVariantTypeInt, msg.GetParam(0).get_type());
	EXPECT_EQ(kVariantTypeFloat, msg.GetParam(1).get_type());
	EXPECT_EQ(kVariantTypeNull, msg.GetParam(2).get_type());
	EXPECT_EQ(kVariantTypeNull, msg.GetParam(3).get_type());
}