#pragma once

#include "matsu/vector_template.h"
#include "konomi/message.h"

//message�� �ٷ�� Ŭ������ ���� ����ϴ� ��ũ�� ����
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
	SetPrimitiveParam<TYPE, INDEX1>(value.GetX());	\
	SetPrimitiveParam<TYPE, INDEX2>(value.GetY());	\
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
	SetPrimitiveParam<TYPE, INDEX1>(value.GetX());	\
	SetPrimitiveParam<TYPE, INDEX2>(value.GetY());	\
	SetPrimitiveParam<TYPE, INDEX3>(value.GetZ());	\
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

		//int, float, bool�� ���� ����Ÿ�Կ� ���ؼ� �۵��ϴ� �Լ�
		//�̰͸� �����ϵ��� �� ������
		//message�� ���ѵ� Ÿ�Ը� �޾Ƶ��ϼ��ִ� Variant�� �������
		//�����Ǿ��ֱ⶧���̴�
		//�̺��� ������ Ÿ���� ���� �̰��� ������� �߰��Լ��� �����ߵȴ�
		template<typename T, int index>
		T GetPrimitiveParam() const;
		template<typename T, int index>
		void SetPrimitiveParam(T value);
		///@brief index�� �Լ� ���ڷ� �޴°��� �θ� <>���� �ٷ� �Լ�ȣ�Ⱑ��
		///�ֳ��ϸ� ���ڿ��� �ٷ� T�� Ÿ���� �˾Ƴ��������ϱ�
		template<typename T>
		void SetPrimitiveParam(int index, T value);
		
	private:
		MessageType *msg_;
	};

	///@brief MessageDecorator�� �����ϳ� Ư�� �ڵ��� �޼����� ����ϴ� ���ڷ�����
	///�޼��� Ÿ�� �������� ������ �ɰŰ��Ƽ� ����
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