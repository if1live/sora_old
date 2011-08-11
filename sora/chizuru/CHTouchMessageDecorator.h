// Ŭnicode please
#pragma once

#include "chizuru/CHInputEvent.h"
#include "konomi/KNMessage.h"
#include "konomi/KNMessageDecorator.h"
#include "konomi/KNVariant.h"
#include "matsu/MTVectorTpl.h"

namespace chizuru
{
	/**
	Touch Message format
	0 : not used(원래 category가 있었으나 code로 옮김
	1 : began / moved / ended
	2 : curr_x
	3 : curr_y
	4 : prev_x
	5 : prev_y
	메세지는 총 6개 이상의 값을 담을수 있어야한다
	*/

	class TouchMessageDecorator :
		public konomi::CodeCheckMessageDecorator<InputMessage::MaxVariantNumber, InputEventCodeTouch> {
	public:
		TouchMessageDecorator(TouchMessageDecorator::MessageType *msg);
		static void TouchBeganMessage(InputMessage &msg, const matsu::ivec2 &currPoint, const matsu::ivec2 &prevPoint);
		static void TouchMovedMessage(InputMessage &msg, const matsu::ivec2 &currPoint, const matsu::ivec2 &prevPoint);
		static void TouchEndedMessage(InputMessage &msg, const matsu::ivec2 &currPoint, const matsu::ivec2 &prevPoint);
	public:
		KONOMI_MSG_ACCESS_ENUM(EventType, getTouchType, setTouchType, 1);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getCurrX, setCurrX, 2);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getCurrY, setCurrY, 3);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getPrevX, setPrevX, 4);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getPrevY, setPrevY, 5);
		KONOMI_MSG_ACCESS_VECTOR2(int, getCurrPoint, setCurrPoint, 2, 3);
		KONOMI_MSG_ACCESS_VECTOR2(int, getPrevPoint, setPrevPoint, 4, 5);
	};
}