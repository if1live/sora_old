#pragma once

#include "matsu/vector_template.h"
#include "konomi/message.h"

//message를 다루는 클래스를 찍어낼때 사용하는 매크로 모음
#define KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(TYPE, GETTER_NAME, SETTER_NAME, INDEX)	\
	TYPE GETTER_NAME() const { return GetPrimitiveParam<TYPE, INDEX>(); }	\
	void SETTER_NAME(TYPE value) { SetPrimitiveParam<TYPE, INDEX>(value); }

#define KONOMI_MSG_ACCESS_VECTOR2(TYPE, GETTER_NAME, SETTER_NAME, INDEX1, INDEX2)	\
	matsu::VectorTemplate<TYPE,2> GETTER_NAME() const	\
{	\
	TYPE a = GetPrimitiveParam<TYPE, INDEX1>();	\
	TYPE b = GetPrimitiveParam<TYPE, INDEX2>();	\
	return matsu::VectorTemplate<TYPE,2>(a, b);	\
}	\
	void SETTER_NAME(const matsu::VectorTemplate<TYPE,2> &value)	\
{	\
	SetPrimitiveParam<TYPE, INDEX1>(value.x());	\
	SetPrimitiveParam<TYPE, INDEX2>(value.y());	\
}

#define KONOMI_MSG_ACCESS_VECTOR3(TYPE, GETTER_NAME, SETTER_NAME, INDEX1, INDEX2, INDEX3)	\
	matsu::VectorTemplate<TYPE,3> GETTER_NAME() const	\
{	\
	TYPE a = GetPrimitiveParam<TYPE, INDEX1>();	\
	TYPE b = GetPrimitiveParam<TYPE, INDEX2>();	\
	TYPE c = GetPrimitiveParam<TYPE, INDEX3>();	\
	return matsu::VectorTemplate<TYPE,3>(a, b, c);	\
}	\
	void SETTER_NAME(const matsu::VectorTemplate<TYPE,3> &value)	\
{	\
	SetPrimitiveParam<TYPE, INDEX1>(value.x());	\
	SetPrimitiveParam<TYPE, INDEX2>(value.y());	\
	SetPrimitiveParam<TYPE, INDEX3>(value.z());	\
}

#define KONOMI_MSG_ACCESS_ENUM(ENUM, GETTER_NAME, SETTER_NAME, INDEX)	\
	ENUM GETTER_NAME() const { return (ENUM)(GetPrimitiveParam<int,INDEX>()); }	\
	void SETTER_NAME(ENUM value) { SetPrimitiveParam<int,INDEX>((int)value); }

namespace konomi
{
	template<unsigned int N>
	class MessageDecorator {
	public:
		typedef Message<N> MessageType;
		typedef MessageDecorator<N> SuperDecoratorType;
		enum {
			MaxParamNum = N,
		};
	public:
		MessageDecorator(MessageType *msg);
		MessageDecorator(MessageType &msg);
		~MessageDecorator();
		void SetMessage(MessageType *msg);
		void SetMessage(MessageType &msg);

		//int, float, bool과 같은 원시타입에 대해서 작동하는 함수
		//이것만 지원하도록 한 이유는
		//message는 제한된 타입만 받아들일수있는 Variant를 기반으로
		//구현되어있기때문이다
		//이보다 복잡한 타입의 경우는 이것을 기반으로 추가함수를 만들어야된다
		template<typename T, int index>
		T GetPrimitiveParam() const;
		template<typename T, int index>
		void SetPrimitiveParam(T value);
		///@brief index를 함수 인자로 받는것을 두면 <>없이 바로 함수호출가능
		///왜냐하면 인자에서 바로 T의 타입을 알아낼수있으니까
		template<typename T>
		void SetPrimitiveParam(int index, T value);
		
	private:
		MessageType *msg_;
	};

	///@brief MessageDecorator와 유사하나 특정 코드의 메세지만 취급하는 데코레이터
	///메세지 타입 안정성에 도움이 될거같아서 만듬
	template<unsigned int N, MessageCode code>
	class CodeCheckMessageDecorator : public MessageDecorator<N> {
	public:
		typedef CodeCheckMessageDecorator<N,code> SuperCodeCheckDecoratorType;
		enum {
			ValidCode = code,
		};
	public:
		CodeCheckMessageDecorator(MessageType *msg);
		CodeCheckMessageDecorator(MessageType &msg);
		~CodeCheckMessageDecorator();
	};
}

#include "konomi/message_decorator.hpp"