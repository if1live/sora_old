// Ŭnicode please
#pragma once

#include "../SoraStdAfx.h"

#include "matsu/MTVector.h"
#include "konomi/KNMessageCodeDictionary.h"
#include "konomi/KNMessage.h"

namespace chizuru
{
	///@brief 키보드,마우스 눌린거 여부표시할때 쓰는 enum
	typedef enum {
		ButtonStatePress,
		ButtonStateRelease,
	} ButtonState;

	typedef enum {
		MouseButtonLeft = 0,
		MouseButtonMiddle,
		MouseButtonRight,
	} MouseButton;

	typedef enum {
		///@brief yaw, roll, pitch
		EventTypeAttitude,
		///@brief raw accelerometer data
		EventTypeAccelerometer,
		///@brief gyro+accelerometer, user core motion
		EventTypeGravity,
		
		EventTypeTouchBegan,
		EventTypeTouchMoved,
		EventTypeTouchEnded,
		
		EventTypeHeading,

		///@def 키보드 명령세부
		EventTypeKeyDown,
		EventTypeKeyUp,
	} EventType;

	/*
	typedef enum {
		EventCategoryTouch = 1000,
		EventCategoryMotion,
		EventCategoryLocation,
		EventCategoryKeyboard,
		EventCategoryMouse,
	} EventCategory;
	*/
	KONOMI_MSG_CODE_REGISTER(InputEventCodeTouch, 1000);
	KONOMI_MSG_CODE_REGISTER(InputEventCodeGravity, 1001);
	KONOMI_MSG_CODE_REGISTER(InputEventCodeAccelermeter, 1002);
	KONOMI_MSG_CODE_REGISTER(InputEventCodeHeading, 1003);
	KONOMI_MSG_CODE_REGISTER(InputEventCodeKeyboard, 1004);
	KONOMI_MSG_CODE_REGISTER(InputEventCodeMouse, 1005);
	KONOMI_MSG_CODE_REGISTER(InputEventCodeAttitude, 1006);


	///@brief input message는 전부 input queue에 넣을수있도록 하기 위해서 각각의 크기는 동일하게 하자
	///어차피 매우 큰것은 등장하지 않을테니 심각한 문제는 없을 것이다
	///6개면 일단은 문제 없을듯?
	typedef konomi::Message<6> InputMessage;
	typedef std::list<InputMessage> InputMessageListType;
	typedef std::queue<InputMessage> InputMessageQueueType;
	
	InputMessage createTouchBeganEvent(const matsu::ivec2 &currPos, const matsu::ivec2 &prevPos);
	InputMessage createTouchMovedEvent(const matsu::ivec2 &currPos, const matsu::ivec2 &prevPos);
	InputMessage createTouchEndedEvent(const matsu::ivec2 &currPos, const matsu::ivec2 &prevPos);
	
	InputMessage createAccelerometerEvent(float x, float y, float z);
	InputMessage createAccelerometerEvent(float x, float y, float z, double timestamp);
	InputMessage createGravityEvent(float x, float y, float z);
	InputMessage createAttitudeEvent(float roll, float pitch, float yaw);
	
	//InputEventPtr createHeadingEvent(float trueHeading, float magneticHeading, float x, float y, float z);

#if _WIN_
	//InputEventPtr createKeyDownEvent(unsigned char key);
	//InputEventPtr createKeyUpEvent(unsigned char key);
#endif
	/*
	class HeadingData {
	public:
		HeadingData(float trueHeading, float magneticHeading, float x, float y, float z);
		float trueHading;
		float magneticHeading;
		float rawX;
		float rawY;
		float rawZ;
		
		static HeadingData getLast();
	private:
		static float lastTrueHeading_;
		static float lastMagneticHeading_;
		static float lastRawX_;
		static float lastRawY_;
		static float lastRawZ_;
	};
	*/
}
