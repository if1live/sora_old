#pragma once

#include "matsu/MTVectorTpl.h"
#include "konomi/KNMessage.h"

//message�� �ٷ�� Ŭ������ ���� ����ϴ� ��ũ�� ����
#define KONOMI_MSG_ACCESS_PRIMITIVE_TYPE(TYPE, GETTER_NAME, SETTER_NAME, INDEX)	\
	TYPE GETTER_NAME() const { return getPrimitiveParam<TYPE, INDEX>(); }	\
	void SETTER_NAME(TYPE value) { setPrimitiveParam<TYPE, INDEX>(value); }

#define KONOMI_MSG_ACCESS_VECTOR2(TYPE, GETTER_NAME, SETTER_NAME, INDEX1, INDEX2)	\
	matsu::VectorTpl<TYPE,2> GETTER_NAME() const	\
{	\
	TYPE a = getPrimitiveParam<TYPE, INDEX1>();	\
	TYPE b = getPrimitiveParam<TYPE, INDEX2>();	\
	return matsu::VectorTpl<TYPE,2>(a, b);	\
}	\
	void SETTER_NAME(const matsu::VectorTpl<TYPE,2> &value)	\
{	\
	setPrimitiveParam<TYPE, INDEX1>(value.getX());	\
	setPrimitiveParam<TYPE, INDEX2>(value.getY());	\
}

#define KONOMI_MSG_ACCESS_VECTOR3(TYPE, GETTER_NAME, SETTER_NAME, INDEX1, INDEX2, INDEX3)	\
	matsu::VectorTpl<TYPE,3> GETTER_NAME() const	\
{	\
	TYPE a = getPrimitiveParam<TYPE, INDEX1>();	\
	TYPE b = getPrimitiveParam<TYPE, INDEX2>();	\
	TYPE c = getPrimitiveParam<TYPE, INDEX3>();	\
	return matsu::VectorTpl<TYPE,3>(a, b, c);	\
}	\
	void SETTER_NAME(const matsu::VectorTpl<TYPE,3> &value)	\
{	\
	setPrimitiveParam<TYPE, INDEX1>(value.getX());	\
	setPrimitiveParam<TYPE, INDEX2>(value.getY());	\
	setPrimitiveParam<TYPE, INDEX3>(value.getZ());	\
}

#define KONOMI_MSG_ACCESS_ENUM(ENUM, GETTER_NAME, SETTER_NAME, INDEX)	\
	ENUM GETTER_NAME() const { return (ENUM)(getPrimitiveParam<int,INDEX>()); }	\
	void SETTER_NAME(ENUM value) { setPrimitiveParam<int,INDEX>((int)value); }

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
		void setMessage(MessageType *msg);
		void setMessage(MessageType &msg);

		//int, float, bool�� ���� ����Ÿ�Կ� ���ؼ� �۵��ϴ� �Լ�
		//�̰͸� �����ϵ��� �� ������
		//message�� ���ѵ� Ÿ�Ը� �޾Ƶ��ϼ��ִ� Variant�� �������
		//�����Ǿ��ֱ⶧���̴�
		//�̺��� ������ Ÿ���� ���� �̰��� ������� �߰��Լ��� �����ߵȴ�
		template<typename T, int index>
		T getPrimitiveParam() const;
		template<typename T, int index>
		void setPrimitiveParam(T value);
		///@brief index�� �Լ� ���ڷ� �޴°��� �θ� <>���� �ٷ� �Լ�ȣ�Ⱑ��
		///�ֳ��ϸ� ���ڿ��� �ٷ� T�� Ÿ���� �˾Ƴ��������ϱ�
		template<typename T>
		void setPrimitiveParma(int index, T value);
		
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

#include "konomi/KNMessageDecorator.hpp"