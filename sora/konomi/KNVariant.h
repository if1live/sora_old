// Ŭnicode please
#pragma once

namespace konomi
{
	typedef enum {
		VariantTypeInt,
		VariantTypeBool,
		VariantTypeFloat,
		VariantTypePointer,
		VariantTypeNull
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