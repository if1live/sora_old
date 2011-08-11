// Ŭnicode please
#pragma once
namespace matsu
{
	template<typename AssocType>
	void destroyDict(AssocType &dict)
	{
		typename AssocType::iterator it = dict.begin();
		typename AssocType::iterator endit = dict.end();
		for( ; it != endit ; it++)
		{
			delete(it->second);
		}
		dict.clear();
	}
	
	template<typename SeqType>
	void destroyList(SeqType &list)
	{
		typename SeqType::iterator it = list.begin();
		typename SeqType::iterator endit = list.end();
		for( ; it != endit ; it++)
		{
			delete(*it);
		}
		list.clear();
	}
}