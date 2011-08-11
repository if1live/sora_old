// Ŭnicode please
#pragma once

#include "matsu/MTUnFixedSizeMemPool.h"

namespace konomi
{
	typedef enum {
		VariantTypeInt,
		VariantTypeBool,
		VariantTypeFloat,
		VariantTypePointer,
		VariantTypeNull
	} VariantType;
	
	//variant의 경우는 구조 자체가 매우 단순하고 잦은 생성파괴가 있을것이다
	//그렇다고 boost pool같은걸 쓰면 api가 new/delete하고 약간다르고
	//variant가 동시에 그렇게 많이 존재할것같진 않으니까
	//(대부분 지역변수일테고 실제로 항상 존재하는것은 얼마 되지 않을것이다)
	//최대갯수를 적당히 잡자. variant자체가 워낙 크기가 작으니까 많이 생성해도
	//심각한 문제는 발생하지 않을것이다.
#define VARIANT_CAPACITY 512
	class Variant : public matsu::UnfixedSizeMemPool<Variant, VARIANT_CAPACITY> {
	public:
	public:
		Variant();
		Variant(int i);
		Variant(float f);
		Variant(bool b);
		template<typename T>
		Variant(T *ptr);
		~Variant();

		VariantType getType() const;

		bool isNull() const;
		void setNull();
		
		operator int() const;
		operator float() const;
		operator bool() const;
		template<typename T>
		T *getAsPointer() const;
		template<typename T>
		T getAsEnum() const;

		void set(int i);
		void set(float f);
		void set(bool b);
		template<typename T>
		void set(T *ptr);

		bool operator==(const Variant &o) const;
		bool operator!=(const Variant &o) const;

	private:
		VariantType type_;
		union {
			int ivalue_;
			float fvalue_;
			bool bvalue_;
			void *pvalue_;
		};
	};

#undef VARIANT_CAPACITY
}

namespace konomi
{
	template<typename T>
	Variant::Variant(T *ptr)
		: pvalue_(ptr), type_(VariantTypePointer)
	{
	}

	template<typename T>
	void Variant::set(T *ptr)
	{
		pvalue_ = ptr;
		type_ = VariantTypePointer;
	}

	template<typename T>
	T *Variant::getAsPointer() const
	{
		return static_cast<T*>(pvalue_);
	}
	template<typename T>
	T Variant::getAsEnum() const
	{
		return static_cast<T>(ivalue_);
	}
}