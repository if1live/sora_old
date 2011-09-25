// Ŭnicode please
#pragma once

#include "../SoraStdAfx.h"
#include "sora/SRSingleton.h"

namespace irina
{
	typedef enum {
		ValueTypeContainer,
		ValueTypeDict,
	} ValueType;

	class AbstractValue {
	public:
		virtual ValueType getType() const = 0;
		virtual bool isNull() const = 0;

		///@brief AbstrctValue를 ValueContainer로 캐스팅한후 값을 얻는것은 심히 귀찮아서
		///최상위 클래스에서 더 간단한 함수를 제공한다. 타입안정성을 위해서 잘못 얻으려고하면 
		///프로그램을 죽이자
		template<typename T>
			const T& get() const;
	};

	class NullValue : public AbstractValue, public sora::Singleton<NullValue> {
	public:
		ValueType getType() const { return ValueTypeContainer; }
		bool isNull() const { return true; }
	};

	template<typename T>
		class ValueContainer : public AbstractValue, boost::noncopyable {
	public:
		ValueContainer(T v);		
		const T& get() const;

		ValueType getType() const { return ValueTypeContainer; }
		bool isNull() const { return false; }
	private:
		T value_;
	};
	
	class ValueDict : public AbstractValue, boost::noncopyable {
	public:
		typedef std::string ValueDictKey;
		typedef std::tr1::unordered_map<ValueDictKey, AbstractValue*> DictType;

		ValueDict();
		~ValueDict();

		ValueType getType() const { return ValueTypeDict; }
		bool isNull() const { return false; }

		void set(const ValueDictKey &key, AbstractValue *value);
		void set(const char *key, AbstractValue* value);

		template<typename T>
			void add(const char *key, T value);
		template<typename T>
			void add(const ValueDictKey &key, T value);

		template<typename T>
			const T& get(const char *key) const;
		template<typename T>
			const T& get(const ValueDictKey &key) const;
		template<typename T>
			const T& getContainer(const char *key) const { return get<T>(key); }
		template<typename T>
			const T& getContainer(const ValueDictKey &key) const { return get<T>(key); }

		AbstractValue* get(const char *key) const;
		AbstractValue* get(const ValueDictKey &key) const;
		//operator overload으로 좀더 쉽게 얻기
		AbstractValue* operator[](const char *key) const;
		AbstractValue* operator[](const ValueDictKey &key) const;

		bool exist(const char *key) const;
		bool exist(const ValueDictKey &key) const;

	private:
		DictType dict_;
	};
}

#include "IRValueDict.hpp"
