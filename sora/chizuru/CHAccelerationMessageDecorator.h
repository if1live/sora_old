// Ŭnicode please
#pragma once
#include "matsu/MTVectorTpl.h"
#include "konomi/KNMessage.h"
#include "konomi/KNMessageDecorator.h"
#include "konomi/KNVariant.h"
#include "chizuru/CHInputEvent.h"

namespace chizuru
{
	/**
	0 : input category(motion)
	1 : input type(acceleration, gravity)
	2 : x(float)
	3 : y(float)
	4 : z(float)
	5 : timestamp(float)
	*/

	//acceleration, gravity는 형태가 동일하지만 따로 존재해야한다.
	//매크로로 찍어내자
	
#define CHIZURU_VEC3_EVENT_MESSAGE_DECORATION(CLASS_NAME, EVT_CATEGORY, EVT_TYPE)	\
	class CLASS_NAME : public konomi::CodeCheckMessageDecorator<InputMessage::MaxVariantNumber, EVT_CATEGORY> {	\
	public:	\
		CLASS_NAME(InputMessage *msg) : konomi::CodeCheckMessageDecorator<InputMessage::MaxVariantNumber,EVT_CATEGORY>(msg) {}	\
		static void Message(InputMessage &msg, float x, float y, float z)	\
		{	\
			using namespace konomi;	\
			msg.clearParam();	\
			lastX_ = x; lastY_ = y;	lastZ_ = z;	\
			CLASS_NAME decorator(&msg);	\
			decorator.setType(EVT_TYPE);	\
			decorator.setX(x);	decorator.setY(y);	decorator.setZ(z);	\
		}	\
		KONOMI_MSG_ACCESS_ENUM(EventType, getType, setType, 1);	\
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getX, setX, 2);	\
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getY, setY, 3);	\
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getZ, setZ, 4);	\
		static matsu::vec3 getLast() \
		{	\
			return matsu::vec3(lastX_, lastY_, lastZ_);	\
		}	\
	private:	\
		static float lastX_;	\
		static float lastY_;	\
		static float lastZ_;	\
	};	\
	float CLASS_NAME::lastX_ = 0;	\
	float CLASS_NAME::lastY_ = 0;	\
	float CLASS_NAME::lastZ_ = 0;

	CHIZURU_VEC3_EVENT_MESSAGE_DECORATION(
		AccelerationMessageDecorator, 
		InputEventCodeAccelermeter,
		chizuru::EventTypeAccelerometer
	);

	CHIZURU_VEC3_EVENT_MESSAGE_DECORATION(
		GravityMessageDecorator, 
		InputEventCodeGravity,
		EventTypeGravity
	);
}