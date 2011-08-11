// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "chizuru/CHTouchMessageDecorator.h"
#include "matsu/MTVectorTpl.h"

using namespace matsu;
using namespace chizuru;
using namespace konomi;

TEST(CHTouchMessageDecorator, createMessage)
{
	ivec2 p1(100, 100);
	ivec2 p2(50, 50);
	InputMessage msg1(InputEventCodeTouch);
	
	TouchMessageDecorator::TouchBeganMessage(msg1, p1, p2);

	TouchMessageDecorator decorator(&msg1);
	EXPECT_EQ(EventTypeTouchBegan, decorator.getTouchType());
	EXPECT_EQ(p1, decorator.getCurrPoint());
	EXPECT_EQ(p2, decorator.getPrevPoint());

	TouchMessageDecorator::TouchMovedMessage(msg1, p1, p2);
	EXPECT_EQ(EventTypeTouchMoved, decorator.getTouchType());

	TouchMessageDecorator::TouchEndedMessage(msg1, p1, p2);
	EXPECT_EQ(EventTypeTouchEnded, decorator.getTouchType());
}