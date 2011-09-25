#include "precompile.h"
#include "konomi/message_decorator.h"
#include "matsu/vector.h"

using namespace matsu;
using namespace std;
using namespace konomi;

TEST(MessageDecorator, set_GetPrimitiveParam)
{
	Message<6> msg;
	MessageDecorator<msg.MaxVariantNumber> decorator(msg);

	int a = 1;
	float b = 2;
	bool c = false;
	decorator.SetPrimitiveParam<int, 1>(a);
	decorator.SetPrimitiveParam<float, 2>(b);
	decorator.SetPrimitiveParam<bool, 0>(c);

	EXPECT_EQ(a, (decorator.GetPrimitiveParam<int, 1>()));
	EXPECT_EQ(b, (decorator.GetPrimitiveParam<float, 2>()));
	EXPECT_EQ(c, (decorator.GetPrimitiveParam<bool, 0>()));
}

typedef enum {
	SampleMessageEnumT1
} SampleMessageEnum;
class SampleMessageDecorator : public MessageDecorator<6> {
public:
	SampleMessageDecorator(MessageType *msg) 
		: MessageDecorator<6>(msg)
	{}

	KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getX, setX, 0);
	KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getFloat, setFloat, 1);
	KONOMI_MSG_ACCESS_VECTOR2(float, getVec2, setVec2, 2, 3);
	KONOMI_MSG_ACCESS_VECTOR3(int, getVec3, setVec3, 2, 3, 5);
	KONOMI_MSG_ACCESS_ENUM(SampleMessageEnum, getEnum, setEnum, 4);
};

TEST(SampleMessageDecorator, macroFunction)
{
	SampleMessageDecorator::MessageType msg;
	SampleMessageDecorator decorator(&msg);

	decorator.setX(1);
	EXPECT_EQ(1, decorator.getX());

	decorator.setFloat(2.0f);
	EXPECT_EQ(2.0f, decorator.getFloat());

	decorator.setVec2(vec2(1, 2));
	EXPECT_EQ(vec2(1, 2), decorator.getVec2());

	decorator.setVec3(ivec3(2, 3, 4));
	EXPECT_EQ(ivec3(2, 3, 4), decorator.getVec3());

	decorator.setEnum(SampleMessageEnumT1);
	EXPECT_EQ(SampleMessageEnumT1, decorator.getEnum());
}

class SampleCodeCheckMessageDecorator : public CodeCheckMessageDecorator<4, 1> {
public:
	SampleCodeCheckMessageDecorator(MessageType *msg)
		: SuperCodeCheckDecoratorType(msg)
	{ }
};

TEST(SampleCodeCheckMessageDecorator, construct)
{
	SampleCodeCheckMessageDecorator::MessageType msg1(0);
	SampleCodeCheckMessageDecorator::MessageType msg2(SampleCodeCheckMessageDecorator::ValidCode);

	//0번 코드 쓴거는 생성불가
	//SampleCodeCheckMessageDecorator decorator1(&msg1);	//assert에 의해서 죽을것
	SampleCodeCheckMessageDecorator decorator2(&msg2);
}