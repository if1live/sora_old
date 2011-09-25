#include "../SoraStdAfx.h"
#include "chizuru/CHMouse.h"

namespace chizuru
{
#if _GLFW_
	void updateMouse_glfw(InputMessageListType &msgList)
	{
		//glfw의 결과를 mouse에 꽂기
		int xpos = 0;
		int ypos = 0;
		glfwGetMousePos(&xpos, &ypos);

		//button 상태 얻기
		ButtonState state[3];	//left, middle, right
		for(int i = 0 ; i < CHIZURU_MOUSE_BTN_NUM ; i++)
		{
			state[i] = ButtonStateRelease;
		}
		if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			state[MouseButtonLeft] = ButtonStateRelease;
		}
		if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
		{
			state[MouseButtonMiddle]= ButtonStateRelease;
		}
		if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			state[MouseButtonRight] = ButtonStateRelease;
		}

		Mouse::getInstance().apply(state, xpos, ypos, msgList);
	}
#endif

	Mouse::Mouse()
	{
		reset();
	}
	Mouse::~Mouse()
	{
	}

	void Mouse::reset()
	{
		prevX_ = 0;
		prevY_ = 0;
		for(int i = 0 ; i < CHIZURU_MOUSE_BTN_NUM ; i++)
		{
			prevState_[i] = ButtonStateRelease;
		}
	}
	bool Mouse::checkMessageOccur(MouseButton btn, ButtonState currState, int currX, int currY, InputMessage &msg)
	{
		//공통부
		MouseMessageDecorator decorator(msg);
		decorator.setButton(btn);
		decorator.setCurrX(currX);
		decorator.setCurrY(currY);
		decorator.setPrevX(prevX_);
		decorator.setPrevY(prevY_);

		ButtonState prevState = prevState_[btn];
		if(prevState == ButtonStateRelease && currState == ButtonStatePress) 
		{
			//began
			decorator.setType(EventTypeTouchBegan);
			return true;
		}
		else if(prevState == ButtonStatePress && currState == ButtonStatePress) 
		{
			//moved
			if(prevX_ != currX || prevY_ != currY) 
			{
				decorator.setType(EventTypeTouchMoved);
				return true;
			}
		} 
		else if(prevState == ButtonStatePress && currState == ButtonStateRelease) 
		{
			//ended
			decorator.setType(EventTypeTouchEnded);
			return true;
		}
		return false;
	}
	void Mouse::apply(ButtonState currState[CHIZURU_MOUSE_BTN_NUM], int currX, int currY, InputMessageListType &msgResult)
	{	
		for(int i = 0 ; i < 3 ; i++)
		{
			InputMessage msg(InputEventCodeMouse);
			MouseButton btn = static_cast<MouseButton>(i);
			bool result = checkMessageOccur(btn, currState[btn], currX, currY, msg);
			if(result == true)
			{
				msgResult.push_back(msg);
			}
		}

		//post apply
		memcpy(prevState_, currState, sizeof(ButtonState) * CHIZURU_MOUSE_BTN_NUM);
		prevX_ = currX;
		prevY_ = currY;
	}

	float Mouse::getX() const
	{
		return prevX_;
	}
	float Mouse::getY() const
	{
		return prevY_;
	}
	ButtonState Mouse::getState(MouseButton btn) const
	{
		SR_ASSERT(btn >= 0 && btn < CHIZURU_MOUSE_BTN_NUM);
		return prevState_[btn];
	}

	//////////////
	MouseMessageDecorator::MouseMessageDecorator(InputMessage &msg)
		: SuperCodeCheckDecoratorType(msg)
	{
	}
	MouseMessageDecorator::~MouseMessageDecorator()
	{
	}
}