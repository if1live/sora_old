// Ŭnicode please
#include "../SoraStdAfx.h"
#include "chizuru/CHAttitudeMessageDecorator.h"

using namespace matsu;

namespace chizuru
{
	AttitudeMessageDecorator::AttitudeMessageDecorator(InputMessage *msg)
		: SuperCodeCheckDecoratorType(msg)
	{
	}

	float AttitudeMessageDecorator::lastRoll_ = 0;
	float AttitudeMessageDecorator::lastPitch_ = 0;
	float AttitudeMessageDecorator::lastYaw_ = 0;
	InputMessage AttitudeMessageDecorator::getLast()
	{
		InputMessage msg;
		AttitudeMessageDecorator decorator(&msg);
		decorator.setType(EventTypeAttitude);
		decorator.setRoll(lastRoll_);
		decorator.setPitch(lastPitch_);
		decorator.setYaw(lastYaw_);
		return msg;
	}

	void AttitudeMessageDecorator::Message(InputMessage &msg, float roll, float pitch, float yaw)
	{
		using namespace konomi;
		msg.clearParam();

		lastRoll_ = roll;
		lastPitch_ = pitch;
		lastYaw_ = yaw;

		AttitudeMessageDecorator decorator(&msg);
		decorator.setType(EventTypeAttitude);
		decorator.setRoll(roll);
		decorator.setPitch(pitch);
		decorator.setYaw(yaw);
	}
}