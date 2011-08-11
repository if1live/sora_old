// Ŭnicode please
#include "../SoraStdAfx.h"
#include "chizuru/CHInputEventQueue.h"
#include "chizuru/CHInputEvent.h"

using namespace std;

namespace chizuru
{
	InputEventQueue::InputEventQueue()
	{
	}
	InputEventQueue::~InputEventQueue()
	{
		//matsu::destroyList(evtQueue_);
		clear();
	}
	InputEventQueue &InputEventQueue::sharedQueue()
	{
		static InputEventQueue queue;
		return queue;
	}

	void InputEventQueue::push(const InputMessage &e)
	{
		evtQueue_.push(e);
	}
	InputMessage InputEventQueue::pop()
	{
		InputMessage evt = evtQueue_.front();
		evtQueue_.pop();
		return evt;
	}
	void InputEventQueue::clear()
	{
		//큐 목록 비우기+이벤트 객체 삭제
		//근데 queue는 iteratore가 없어서 라이브러리 없이 적절히 수동으로 지워야한다
		while(evtQueue_.empty() == false)
		{
			InputMessage evt = evtQueue_.front();
			evtQueue_.pop();
		}
	}
	
	bool InputEventQueue::isEmpty() const
	{
		return evtQueue_.empty();
	}
}