#include "../SoraTestStdAfx.h"
#include "matsu/MTMacro.h"
#include "chizuru/CHInputEvent.h"
#include "chizuru/CHKeyboardEvent.h"

using namespace chizuru;
/*
TEST(InputEvent, create)
{
	matsu::ivec2 prev;
	matsu::ivec2 curr;
	
	InputEventPtr e = createTouchBeganEvent(prev, curr);
	EXPECT_EQ(EventTypeTouchBegan, e->getType());
	EXPECT_EQ(EventCategoryTouch, e->getCategory());
	
	e = createTouchMovedEvent(prev, curr);
	EXPECT_EQ(EventTypeTouchMoved, e->getType());
	EXPECT_EQ(EventCategoryTouch, e->getCategory());
	
	e = createTouchEndedEvent(prev, curr);
	EXPECT_EQ(EventTypeTouchEnded, e->getType());
	EXPECT_EQ(EventCategoryTouch, e->getCategory());
	
	e = createAccelerometerEvent(1, 1, 1);
	EXPECT_EQ(EventTypeAccelerometer, e->getType());
	EXPECT_EQ(EventCategoryMotion, e->getCategory());
	
	e = createGravityEvent(1, 2, 3);
	EXPECT_EQ(EventTypeGravity, e->getType());
	EXPECT_EQ(EventCategoryMotion, e->getCategory());
}

#if _WIN_
TEST(InputEvnet, keyboardEvent)
{
	//keyboard event test
	InputEventPtr e = createKeyDownEvent('a');
	EXPECT_EQ(EventTypeKeyDown, e->getType());
	EXPECT_EQ(EventCategoryKeyboard, e->getCategory());
	KeyboardData *data = e->getEventData<KeyboardData>();
	EXPECT_EQ('a', data->key);

	e = createKeyUpEvent('b');
	EXPECT_EQ(EventTypeKeyUp, e->getType());
	EXPECT_EQ(EventCategoryKeyboard, e->getCategory());
	data = e->getEventData<KeyboardData>();
	EXPECT_EQ('b', data->key);
}
#endif
*/