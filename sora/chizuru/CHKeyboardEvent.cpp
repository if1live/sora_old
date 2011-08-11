// Ŭnicode please
#include "../SoraStdAfx.h"
#if _WIN_

#include "chizuru/CHKeyboardEvent.h"

namespace chizuru
{
	KeyboardData::KeyboardData(unsigned char key)
	: key(key)
	{
	}
	KeyboardData KeyboardData::getLast()
	{
		return KeyboardData(lastKey_);
	}
	unsigned char KeyboardData::lastKey_ = ' ';
}
#endif