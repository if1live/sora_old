// Ŭnicode please
#pragma once

///@brief 상속받아서 바로쓰는 간단한 타입 시스템
#define MT_SUPER_CLASS_DEF	\
virtual const std::string &getType() = 0;

#define MT_CLASS_TYPE(TYPE)	\
virtual const std::string &getType() { return Type(); }	\
static const std::string &Type() { static std::string t(TYPE); return t; }