// Ŭnicode please
#include "../SoraStdAfx.h"
#include "chizuru/CHInputEvent.h"
#include "chizuru/CHKeyboardEvent.h"
#include "chizuru/CHAccelerationMessageDecorator.h"
#include "chizuru/CHTouchMessageDecorator.h"
#include "chizuru/CHAttitudeMessageDecorator.h"

namespace chizuru
{	
	InputMessage createTouchBeganEvent(const matsu::ivec2 &currPos, const matsu::ivec2 &prevPos)
	{
		InputMessage msg(InputEventCodeTouch);
		TouchMessageDecorator::TouchBeganMessage(msg, currPos, prevPos);
		return msg;
	}
	InputMessage createTouchMovedEvent(const matsu::ivec2 &currPos, const matsu::ivec2 &prevPos)
	{
		InputMessage msg(InputEventCodeTouch);
		TouchMessageDecorator::TouchMovedMessage(msg, currPos, prevPos);
		return msg;
	}
	InputMessage createTouchEndedEvent(const matsu::ivec2 &currPos, const matsu::ivec2 &prevPos)
	{
		InputMessage msg(InputEventCodeTouch);
		TouchMessageDecorator::TouchEndedMessage(msg, currPos, prevPos);
		return msg;
	}
	
	InputMessage createAccelerometerEvent(float x, float y, float z)
	{
		InputMessage msg(InputEventCodeAccelermeter);
		AccelerationMessageDecorator::Message(msg, x, y, z);
		return msg;
	}
	InputMessage createAccelerometerEvent(float x, float y, float z, double timestamp)
	{
		InputMessage msg(InputEventCodeAccelermeter);
		AccelerationMessageDecorator::Message(msg, x, y, z);
		return msg;
	}
	InputMessage createGravityEvent(float x, float y, float z)
	{
		InputMessage msg(InputEventCodeGravity);
		GravityMessageDecorator::Message(msg, x, y, z);
		return msg;
	}
	
	InputMessage createAttitudeEvent(float roll, float pitch, float yaw)
	{
		InputMessage msg(InputEventCodeAttitude);
		AttitudeMessageDecorator::Message(msg, roll, pitch, yaw);
		return msg;
	}
	/*
	InputMessage createHeadingEvent(float trueHeading, float magneticHeading, float x, float y, float z)
	{
		HeadingData *data = new HeadingData(trueHeading, magneticHeading, x, y, z);
		return InputEventPtr(new InputEvent(EventTypeHeading, EventCategoryLocation, (void*)data));
	}
	*/
#if _WIN_
	/*
	InputEventPtr createKeyDownEvent(unsigned char key)
	{
		KeyboardData *data = new KeyboardData(key);
		return InputEventPtr(new InputEvent(EventTypeKeyDown, EventCategoryKeyboard, (void*)data));
	}
	InputEventPtr createKeyUpEvent(unsigned char key)
	{
		KeyboardData *data = new KeyboardData(key);
		return InputEventPtr(new InputEvent(EventTypeKeyUp, EventCategoryKeyboard, (void*)data));
	}
	*/
#endif
	/*
	HeadingData::HeadingData(float trueHeading, float magneticHeading, float x, float y, float z)
	: trueHading(trueHeading), magneticHeading(magneticHeading), rawX(x), rawY(y), rawZ(z)
	{
		lastTrueHeading_ = trueHading;
		lastMagneticHeading_ = magneticHeading;
		lastRawX_ = x;
		lastRawY_ = y;
		lastRawZ_ = z;
	}
	HeadingData HeadingData::getLast()
	{
		return HeadingData(lastTrueHeading_, lastMagneticHeading_, lastRawX_, lastRawY_, lastRawZ_);
	}
	float HeadingData::lastTrueHeading_ = 0;
	float HeadingData::lastMagneticHeading_ = 0;
	float HeadingData::lastRawX_ = 0;
	float HeadingData::lastRawY_ = 0;
	float HeadingData::lastRawZ_ = 0;
	*/
}