#pragma once

#include "sora/SRSingleton.h"
#include "chizuru/CHInputEvent.h"
#include "konomi/KNMessageDecorator.h"

#define CHIZURU_MOUSE_BTN_NUM 3
namespace chizuru
{
	///@brief input message�� �ܺο��� �޵����Ѱ� �ܺο��� ����ť�� ������ ����ť��������
	///���Ҽ��ִ� ���ɼ��� ���ܵα����ؼ��̴�
#if _GLFW_
	void updateMouse_glfw(InputMessageListType &msgList);
#elif _GLUT_
	
#endif

	class Mouse : public sora::Singleton<Mouse> {
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