#include "../SoraStdAfx.h"
#include "chizuru/CHClock.h"

namespace chizuru
{

	Stopwatch::Stopwatch()
		: _start(0), _result(0)
	{
	}
	
	void Stopwatch::start()
	{
		_start = (unsigned int)time(NULL);
	}

	unsigned int Stopwatch::stop()
	{
		unsigned int now = (unsigned int)time(NULL);
		_result = now - _start;
		return _result;
	}
	unsigned int Stopwatch::getTime()
	{
		return _result;
	}
}
