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
	2 : roll(float)
	3 : pitch(float)
	4 : yaw(float)
	5 : timestamp(float)
	*/

	class AttitudeMessageDecorator : public konomi::CodeCheckMessageDecorator<InputMessage::MaxVariantNumber, InputEventCodeAttitude> {
	public:
		AttitudeMessageDecorator(InputMessage *msg);
		static void Message(InputMessage &msg, float roll, float pitch, float yaw);
		static InputMessage getLast();

		KONOMI_MSG_ACCESS_ENUM(EventType, getType, setType, 1);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getRoll, setRoll, 2);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getPitch, setPitch, 3);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(float, getYaw, setYaw, 4);
	private:
		static float lastRoll_;
		static float lastPitch_;
		static float lastYaw_;
	};
}
