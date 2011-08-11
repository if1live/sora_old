#include "../SoraTestStdAfx.h"
#include "chizuru/CHMouse.h"

using namespace chizuru;
using namespace matsu;

TEST(CHMouse, apply_BeganMovedEnded)
{
	Mouse &m = Mouse::getInstance();
	EXPECT_EQ(ButtonStateRelease, m.getState(MouseButtonLeft));
	EXPECT_EQ(ButtonStateRelease, m.getState(MouseButtonMiddle));
	EXPECT_EQ(ButtonStateRelease, m.getState(MouseButtonRight));

	ButtonState state[3];
	state[MouseButtonLeft] = ButtonStatePress;
	state[MouseButtonMiddle] = ButtonStateRelease;
	state[MouseButtonRight] = ButtonStateRelease;

	InputMessageListType msgList;

	m.apply(state, 1, 2, msgList);
	EXPECT_EQ(ButtonStatePress, m.getState(MouseButtonLeft));
	EXPECT_EQ(ButtonStateRelease, m.getState(MouseButtonMiddle));
	EXPECT_EQ(ButtonStateRelease, m.getState(MouseButtonRight));
	EXPECT_EQ(1, m.getX());
	EXPECT_EQ(2, m.getY());

	//left:release->press, evt occur
	EXPECT_EQ(1, msgList.size());
	MouseMessageDecorator decorator(msgList.front());
	EXPECT_EQ(InputEventCodeMouse, msgList.front().getCode());
	EXPECT_EQ(MouseButtonLeft, decorator.getButton());
	EXPECT_EQ(EventTypeTouchBegan, decorator.getType());
	EXPECT_EQ(vec2(0, 0), decorator.getPrevPoint());
	EXPECT_EQ(vec2(1, 2), decorator.getCurrPoint());

	//left:press->press, moved
	msgList.clear();
	m.apply(state, 3, 4, msgList);
	EXPECT_EQ(1, msgList.size());
	decorator.setMessage(msgList.front());
	EXPECT_EQ(InputEventCodeMouse, msgList.front().getCode());
	EXPECT_EQ(MouseButtonLeft, decorator.getButton());
	EXPECT_EQ(EventTypeTouchMoved, decorator.getType());
	EXPECT_EQ(vec2(1, 2), decorator.getPrevPoint());
	EXPECT_EQ(vec2(3, 4), decorator.getCurrPoint());

	//press->release
	state[MouseButtonLeft] = ButtonStateRelease;
	msgList.clear();
	m.apply(state, 3, 4, msgList);
	EXPECT_EQ(1, msgList.size());
	decorator.setMessage(msgList.front());
	EXPECT_EQ(InputEventCodeMouse, msgList.front().getCode());
	EXPECT_EQ(MouseButtonLeft, decorator.getButton());
	EXPECT_EQ(EventTypeTouchEnded, decorator.getType());
}