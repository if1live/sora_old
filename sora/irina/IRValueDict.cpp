// Ŭnicode please
#include "../SoraStdAfx.h"
#include "IRValueDict.h"
#include "sora/SRTemplateLib.h"

using namespace sora;

namespace irina
{

	ValueDict::ValueDict()
	{
	}
	ValueDict::~ValueDict()
	{
		//dict에 엮인것을 해제
		destroyDict(dict_);
	}


	void ValueDict::set(const ValueDictKey &key, AbstractValue *value)
	{
		//이미 존재하면 이전것을 해제한다
		DictType::iterator it;
		it = dict_.find(key);
		if(it != dict_.end())
		{
			delete(it->second);
			dict_.erase(it);
		}
		dict_[key] = value;
	}
	void ValueDict::set(const char *key, AbstractValue *value)
	{
		ValueDictKey keystr(key);
		set(keystr, value);
	}
	AbstractValue* ValueDict::get(const char *key) const
	{
		ValueDictKey keystr(key);
		return get(keystr);
	}
	AbstractValue* ValueDict::get(const ValueDictKey &key) const
	{
		DictType::const_iterator it;
		it = dict_.find(key);
		if(it == dict_.end())
		{
			//return NULL;
			return &(NullValue::getInstance());
		}
		//else...
		return it->second;
	}
	bool ValueDict::exist(const char *key) const
	{
		ValueDictKey keystr(key);
		return exist(keystr);
	}
	bool ValueDict::exist(const ValueDictKey &key) const
	{
		DictType::const_iterator it;
		it = dict_.find(key);
		if(it == dict_.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	AbstractValue* ValueDict::operator[](const char *key) const
	{
		return get(key);
	}
	AbstractValue* ValueDict::operator[](const ValueDictKey &key) const
	{
		return get(key);
	}
}
