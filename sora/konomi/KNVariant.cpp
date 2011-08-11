// Ŭnicode please
#include "../SoraStdAfx.h"
#include "konomi/KNVariant.h"
#include "matsu/MTMacro.h"

namespace konomi
{
	Variant::Variant() : ivalue_(0), type_(VariantTypeNull) {}
	Variant::Variant(int i) : ivalue_(i), type_(VariantTypeInt) {}
	Variant::Variant(float f) : fvalue_(f), type_(VariantTypeFloat) {}
	Variant::Variant(bool b) : bvalue_(b), type_(VariantTypeBool) {}
	Variant::~Variant() {}

	void Variant::setNull()
	{
		type_ = VariantTypeNull;
	}
	bool Variant::isNull() const
	{
		return (type_ == VariantTypeNull);
	}
	VariantType Variant::getType() const
	{
		return type_;
	}
	Variant::operator int() const
	{
		MT_ASSERT(getType() == VariantTypeInt);
		return ivalue_;
	}
	Variant::operator float() const
	{
		MT_ASSERT(getType() == VariantTypeFloat);
		return fvalue_;
	}

	Variant::operator bool() const
	{
		MT_ASSERT(getType() == VariantTypeBool);
		return bvalue_;
	}

	void Variant::set(int i)
	{
		type_ = VariantTypeInt;
		ivalue_ = i;
	}
	void Variant::set(float f)
	{
		type_ = VariantTypeFloat;
		fvalue_ = f;
	}
	void Variant::set(bool b)
	{
		type_ = VariantTypeBool;
		bvalue_ = b;
	}

	bool Variant::operator==(const Variant &o) const
	{
		if(type_ != o.type_)
		{
			return false;
		}
		switch(type_) {
		case VariantTypeInt:
			return (ivalue_ == o.ivalue_);
			break;
		case VariantTypeFloat:
			return (fvalue_ == o.fvalue_);
			break;
		case VariantTypeBool:
			return (bvalue_ == o.bvalue_);
			break;
		case VariantTypePointer:
			return (pvalue_ == o.pvalue_);
			break;
		default:
			MT_ASSERT(!"do not reach");
			return false;
		};
	}
	bool Variant::operator!=(const Variant &o) const
	{
		return !(*this == o);
	}
}
