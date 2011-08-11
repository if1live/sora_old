#include "../SoraTestStdAfx.h"
#include <chizuru/CHInputEventQueue.h>
#include <chizuru/CHInputEvent.h>

using namespace std;
using namespace chizuru;

/*
TEST(InputEventQueue, pushPop)
{
	InputEventQueue q;
	EXPECT_TRUE(q.isEmpty());
	
	InputEventPtr e1 = createAccelerometerEvent(1, 1, 1);
	InputEventPtr e2 = createAccelerometerEvent(1, 1, 2);
	InputEventPtr e3 = createAccelerometerEvent(1, 2, 1);
	q.push(e1);
	q.push(e2);
	q.push(e3);
	EXPECT_FALSE(q.isEmpty());
	
	//pop test
	EXPECT_EQ(e1, q.pop());
	EXPECT_EQ(e2, q.pop());
	EXPECT_EQ(e3, q.pop());
	EXPECT_TRUE(q.isEmpty());
}

TEST(InputEventQueue, clear)
{
	InputEventQueue q;
	EXPECT_TRUE(q.isEmpty());
	
	//생성해서 넣기
	q.push(createAccelerometerEvent(1, 1, 1));
	q.push(createAccelerometerEvent(1, 2, 1));
	EXPECT_FALSE(q.isEmpty());
	
	//clear!
	q.clear();
	EXPECT_TRUE(q.isEmpty());
}
*/