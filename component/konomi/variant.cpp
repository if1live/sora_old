// Ŭnicode please
#include "precompile.h"
#include "konomi/variant.h"
#include "sora/macro.h"

namespace konomi
{
  Variant::Variant() : ivalue_(0), type_(kVariantTypeNull) {}
  Variant::Variant(int i) : ivalue_(i), type_(kVariantTypeInt) {}
  Variant::Variant(float f) : fvalue_(f), type_(kVariantTypeFloat) {}
  Variant::Variant(bool b) : bvalue_(b), type_(kVariantTypeBool) {}
  Variant::~Variant() {}

  void Variant::SetNull()
  {
    type_ = kVariantTypeNull;
  }
  bool Variant::IsNull() const
  {
    return (type_ == kVariantTypeNull);
  }
  VariantType Variant::type() const
  {
    return type_;
  }
  Variant::operator int() const
  {
    SR_ASSERT(type() == kVariantTypeInt);
    return ivalue_;
  }
  Variant::operator float() const
  {
    SR_ASSERT(type() == kVariantTypeFloat);
    return fvalue_;
  }

  Variant::operator bool() const
  {
    SR_ASSERT(type() == kVariantTypeBool);
    return bvalue_;
  }

  Variant& Variant::operator=(int i)
  {
    type_ = kVariantTypeInt;
    ivalue_ = i;
    return *this;
  }
  Variant& Variant::operator=(float f)
  {
    type_ = kVariantTypeFloat;
    fvalue_ = f;
    return *this;
  }
  Variant& Variant::operator=(bool b)
  {
    type_ = kVariantTypeBool;
    bvalue_ = b;
    return *this;
  }

  bool Variant::operator==(const Variant &o) const
  {
    if(type_ != o.type_)
    {
      return false;
    }
    switch(type_) {
    case kVariantTypeInt:
      return (ivalue_ == o.ivalue_);
      break;
    case kVariantTypeFloat:
      return (fvalue_ == o.fvalue_);
      break;
    case kVariantTypeBool:
      return (bvalue_ == o.bvalue_);
      break;
    case kVariantTypePointer:
      return (pvalue_ == o.pvalue_);
      break;
    default:
      SR_ASSERT(!"do not reach");
      return false;
    };
  }
  bool Variant::operator!=(const Variant &o) const
  {
    return !(*this == o);
  }
}
