// Ŭnicode please
#pragma once

#include "chizuru/CHInputEvent.h"

namespace chizuru
{	
	///이벤트큐는 전역용/지역용이 있어야 큐를 비우는 시점같은것을 유연하게 조절할수 있다
	class InputEventQueue {
	public:
		InputEventQueue();
		~InputEventQueue();
		
		void push(const InputMessage &e);
		InputMessage pop();
		
		///@brief 이벤트큐에 들어있는 이벤트를 통쨰로 없애기 
		void clear();
		
		bool isEmpty() const;

		//shared
		static InputEventQueue &sharedQueue();
		
	private:
		std::queue<InputMessage> evtQueue_;
	};
}