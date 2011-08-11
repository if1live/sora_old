#pragma once

#include "matsu/MTSingleton.h"
#include "chizuru/CHInputEvent.h"
#include "konomi/KNMessageDecorator.h"

#define CHIZURU_MOUSE_BTN_NUM 3
namespace chizuru
{
	///@brief input message를 외부에서 받도록한건 외부에서 공용큐에 넣을지 지역큐에넣을지
	///정할수있는 가능성을 남겨두기위해서이다
	void updateMouse_glfw(InputMessageListType &msgList);

	class Mouse : public matsu::Singleton<Mouse> {
	public:
		Mouse();
		virtual ~Mouse();

		void apply(ButtonState currState[CHIZURU_MOUSE_BTN_NUM], int currX, int currY, InputMessageListType &msgResult);

		float getX() const;
		float getY() const;
		ButtonState getState(MouseButton btn) const;
		void reset();
	private:
		bool checkMessageOccur(MouseButton btn, ButtonState currState, int currX, int currY, InputMessage &msg);
		ButtonState prevState_[CHIZURU_MOUSE_BTN_NUM];	//left, middle, right
		int prevX_;
		int prevY_;
	};

	class MouseMessageDecorator : public konomi::CodeCheckMessageDecorator<InputMessage::MaxVariantNumber, InputEventCodeMouse> {
	public:
		MouseMessageDecorator(InputMessage &msg);
		~MouseMessageDecorator();

		KONOMI_MSG_ACCESS_ENUM(MouseButton, getButton, setButton, 0);
		KONOMI_MSG_ACCESS_ENUM(EventType, getType, setType, 1);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getCurrX, setCurrX, 2);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getCurrY, setCurrY, 3);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getPrevX, setPrevX, 4);
		KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(int, getPrevY, setPrevY, 5);
		KONOMI_MSG_ACCESS_VECTOR2(int, getCurrPoint, setCurrPoint, 2, 3);
		KONOMI_MSG_ACCESS_VECTOR2(int, getPrevPoint, setPrevPoint, 4, 5);
	};
}