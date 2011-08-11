// Ŭnicode please
#pragma once

#include <string>
#include <vector>
#include "matsu/MTType.h"

namespace matsu
{
	///@brief string wrapper class
	template<typename _Tstring, typename _Tchar>
	class StringT {
	public:
		StringT() {}
		StringT(const _Tchar *str) : str_(str) {}
		StringT(const _Tstring& str) : str_(str) {}
		StringT(const StringT &str) : str_(str.str_) {}
		~StringT() {}

///operator overloading
		const _Tstring &operator()();

		StringT &operator=(const _Tchar *o);
		StringT &operator=(const _Tstring &o);
		StringT &operator=(const StringT &o);

		bool operator==(const _Tchar *o) const;
		bool operator==(const _Tstring &o) const;
		bool operator==(const StringT &o) const;

		bool operator!=(const _Tchar *o) const;
		bool operator!=(const _Tstring &o) const;
		bool operator!=(const StringT &o) const;

		StringT operator+(const _Tchar *o) const;
		StringT operator+(const _Tstring &o) const;
		StringT operator+(const StringT &o) const;

		StringT &operator+=(const _Tchar *o);
		StringT &operator+=(const _Tstring &o);
		StringT &operator+=(const StringT &o);

		//casting
		operator const _Tstring&() const { return str_; }
		operator const _Tchar&() const { return str_.c_str(); }

		//array subscript
		_Tchar& operator[](const u32 &i) { return str_[i]; }

		u32 length() const { return str_.length(); }

	private:
		_Tstring str_;
	};
}

namespace matsu
{
	template<typename _Tstring, typename _Tchar>
	const _Tstring &StringT<_Tstring, _Tchar>::operator()() { return str_; }

	template<typename _Tstring, typename _Tchar>
	bool StringT<_Tstring, _Tchar>::operator==(const _Tchar *o) const
	{
		_Tstring tmp(o);
		return str_ == tmp;
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> &StringT<_Tstring, _Tchar>::operator=(const _Tchar *o)
	{
		str_ = o;
		return *this;
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> &StringT<_Tstring, _Tchar>::operator=(const _Tstring &o)
	{
		str_ = o;
		return *this;
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> &StringT<_Tstring, _Tchar>::operator=(const StringT &o)
	{
		str_ = o.str_;
		return *this;
	}
	
	template<typename _Tstring, typename _Tchar>
	bool StringT<_Tstring, _Tchar>::operator==(const _Tstring &o) const
	{
		return str_ == o;
	}
	template<typename _Tstring, typename _Tchar>
	bool StringT<_Tstring, _Tchar>::operator==(const StringT &o) const
	{
		return str_ == o.str_;
	}
	template<typename _Tstring, typename _Tchar>
	bool StringT<_Tstring, _Tchar>::operator!=(const _Tchar *o) const
	{
		return str_ != o;
	}
	template<typename _Tstring, typename _Tchar>
	bool StringT<_Tstring, _Tchar>::operator!=(const _Tstring &o) const
	{
		return str_ != o;
	}
	template<typename _Tstring, typename _Tchar>
	bool StringT<_Tstring, _Tchar>::operator!=(const StringT &o) const
	{
		return str_ != o.str_;
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> StringT<_Tstring, _Tchar>::operator+(const _Tchar *o) const
	{
		_Tstring tmp = str_ + o;
		return StringT(tmp);
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> StringT<_Tstring, _Tchar>::operator+(const _Tstring &o) const
	{
		_Tstring tmp = str_ + o;
		return StringT(tmp);
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> StringT<_Tstring, _Tchar>::operator+(const StringT &o) const
	{
		_Tstring tmp = str_ + o.str_;
		return StringT(tmp);
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> &StringT<_Tstring, _Tchar>::operator+=(const _Tchar *o)
	{
		str_ += o;
		return *this;
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> &StringT<_Tstring, _Tchar>::operator+=(const _Tstring &o)
	{
		str_ += o;
		return *this;
	}
	template<typename _Tstring, typename _Tchar>
	StringT<_Tstring, _Tchar> &StringT<_Tstring, _Tchar>::operator+=(const StringT &o)
	{
		str_ += o.str_;
		return *this;
	}

	
	//StringT typedef
	typedef StringT<std::string, char> String;
	typedef StringT<std::wstring, wchar_t> WString;
}
