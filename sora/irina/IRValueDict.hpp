// Ŭnicode please
#pragma once

namespace irina
{
	template<typename T>
	const T& AbstractValue::get() const
	{
		if(getType() != ValueTypeContainer)
		{
			MT_ASSERT(!"not container");
		}
		ValueContainer<T> *container = (ValueContainer<T>*)(this);
		return container->get();
	}
	
	template<typename T>
	ValueContainer<T>::ValueContainer(T v)
		: value_(v)
	{
	}

	template<typename T>
	const T& ValueContainer<T>::get() const
	{
		return value_;
	}
	
	template<typename T>
	void ValueDict::add(const char *key, T value)
	{
		ValueContainer<T> *container = new ValueContainer<T>(value);
		this->set(key, container);
 	}
	template<typename T>
	void ValueDict::add(const ValueDictKey &key, T value)
	{
		ValueContainer<T> *container = new ValueContainer<T>(value);
		this->set(key, container);
	}
	template<typename T>
	const T& ValueDict::get(const char *key) const
	{
		ValueDictKey keystr(key);
		return get<T>(keystr);
	}

	template<typename T>
	const T& ValueDict::get(const ValueDictKey &key) const
	{
		typename DictType::const_iterator it;
		it = dict_.find(key);
		if(it == dict_.end())
		{
			//it==end인 경우 null 객체 반환
			MT_ASSERT(!"key no exist");
		}
		if(it->second->getType() == ValueTypeContainer)
		{
			ValueContainer<T> &container = static_cast<ValueContainer<T>&>(*(it->second));
			return container.get();
		}
		//else
		//value type이 contaier가 아닌 경우 캐스팅도 안되니까 그냥 null value
		MT_ASSERT(!"value type is not container");
		return (const T&)(NULL);
	}

}
