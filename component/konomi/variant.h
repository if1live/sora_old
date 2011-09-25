// Ŭnicode please
#pragma once

namespace konomi
{
	typedef enum {
		kVariantTypeInt,
		kVariantTypeBool,
		kVariantTypeFloat,
		kVariantTypePointer,
		kVariantTypeNull
	} VariantType;
	
	class Variant {
	public:
	public:
		Variant();
		Variant(int i);
		Variant(float f);
		Variant(bool b);
		template<typename T>
		Variant(T *ptr);
		~Variant();

		VariantType get_type() const;

		bool IsNull() const;
		void SetNull();
		
		operator int() const;
		operator float() const;
		operator bool() const;
		template<typename T>
		T *GetAsPointer() const;
		template<typename T>
		T GetAsEnum() const;

    Variant& operator=(int i);
    Variant& operator=(float f);
    Variant& operator=(bool b);
		template<typename T>
		Variant& operator=(T *ptr);

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
}

namespace konomi
{
	template<typename T>
	Variant::Variant(T *ptr)
		: pvalue_(ptr), type_(kVariantTypePointer)
	{
	}

	template<typename T>
  Variant& Variant::operator=(T *ptr)
	{
		pvalue_ = ptr;
		type_ = kVariantTypePointer;
    return *this;
	}

	template<typename T>
	T *Variant::GetAsPointer() const
	{
		return static_cast<T*>(pvalue_);
	}
	template<typename T>
	T Variant::GetAsEnum() const
	{
		return static_cast<T>(ivalue_);
	}
}