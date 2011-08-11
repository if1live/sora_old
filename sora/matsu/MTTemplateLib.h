﻿// Ŭnicode please
#pragma once

namespace matsu
{
	///@brief second에 포인터를 저장하는 연관 컨테이너가 보관하는 포인터 해제 및 적절히 비우기
	template<typename AssocType>
	void destroyDict(AssocType &dict);
	
	///@brief 포인터를 저장하는 컨테이너의 내용 delete 및 비우기
	template<typename SeqType>
	void destroyList(SeqType &list);
}

#include "matsu/MTTemplateLib.hpp"