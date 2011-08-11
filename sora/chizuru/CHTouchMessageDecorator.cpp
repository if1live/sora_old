#include "../SoraStdAfx.h"
#include "chizuru/CHTouchMessageDecorator.h"

namespace chizuru
{
	
	TouchMessageDecorator::TouchMessageDecorator(TouchMessageDecorator::MessageType *msg)
		: SuperCodeCheckDecoratorType(msg)
	{
	}
	
	void TouchMessageDecorator::TouchBeganMessage(InputMessage &msg, const matsu::ivec2 &currPoint, const matsu::ivec2 &prevPoint)
	{
		using namespace konomi;
		msg.clearParam();

		TouchMessageDecorator decorator(&msg);
		decorator.setTouchType(EventTypeTouchBegan);
		decorator.setCurrX(currPoint.getX());
		decorator.setCurrY(currPoint.getY());
		decorator.setPrevX(prevPoint.getX());
		decorator.setPrevY(prevPoint.getY());
	}
	void TouchMessageDecorator::TouchMovedMessage(InputMessage &msg, const matsu::ivec2 &currPoint, const matsu::ivec2 &prevPoint)
	{
		using namespace konomi;
		msg.clearParam();
		
		TouchMessageDecorator decorator(&msg);
		decorator.setTouchType(EventTypeTouchMoved);
		decorator.setCurrX(currPoint.getX());
		decorator.setCurrY(currPoint.getY());
		decorator.setPrevX(prevPoint.getX());
		decorator.setPrevY(prevPoint.getY());
	}
	
	void TouchMessageDecorator::TouchEndedMessage(InputMessage &msg, const matsu::ivec2 &currPoint, const matsu::ivec2 &prevPoint)
	{
		using namespace konomi;
		msg.clearParam();
		
		TouchMessageDecorator decorator(&msg);
		decorator.setTouchType(EventTypeTouchEnded);
		decorator.setCurrX(currPoint.getX());
		decorator.setCurrY(currPoint.getY());
		decorator.setPrevX(prevPoint.getX());
		decorator.setPrevY(prevPoint.getY());
	}
}