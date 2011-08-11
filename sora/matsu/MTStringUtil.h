// Ŭnicode please
#pragma once

#include "matsu/MTString.h"
#include "matsu/MTType.h"
#include "matsu/MTMacro.h"

//use boost
//#include <boost/algorithm/string.hpp>
//#include <boost/foreach.hpp>

namespace matsu
{
	///@brief std::string, std::wstring operator collection class
	template<typename _Tstring, typename _Tchar>
	class StringTUtil {
	public:
		typedef StringT<_Tstring, _Tchar> StringTType;

		static StringT<_Tstring, _Tchar> trim(const StringT<_Tstring, _Tchar> &str)
		{
			return StringTType(trim(static_cast<const _Tstring&>(str)));
		}

		static _Tstring trim(const _Tstring &str)
		{
			//do no use boost to compile speed + dependenty
			const _Tchar *whitespace = "\t\n\x0b\x0c\r ";

			u32 leftFound = str.find_first_not_of(whitespace);
			//_Tstring left(n == _Tstring::npos ? tmp : tmp.substr(n, tmp.length()));
			u32 rightFound = str.find_last_not_of(whitespace);
			//_Tstring right(n == _Tstring::npos ? tmp : tmp.substr(0, n + 1));

			if(leftFound == _Tstring::npos)
			{
				leftFound = 0;
			}
			if(rightFound == _Tstring::npos)
			{
				rightFound = str.length();
			}
			const _Tstring result = str.substr(leftFound, rightFound-leftFound+1);
			return result;
		}
		static StringT<_Tstring, _Tchar> leftTrim(const StringTType &str)
		{
			return StringTType(leftTrim(static_cast<const _Tstring&>(str)));
		}
		static _Tstring leftTrim(const _Tstring &str)
		{
			u32 n = str.find_first_not_of("\t\n\x0b\x0c\r ");
			_Tstring left(n == _Tstring::npos ? str : str.substr(n, str.length()));
			//_Tstring left = boost::trim_left_copy(str_);
			return left;

		}
		static StringT<_Tstring, _Tchar> rightTrim(const StringTType &str) 
		{
			return StringTType(rightTrim(static_cast<const _Tstring&>(str)));
		}
		static _Tstring rightTrim(const _Tstring &str)
		{
			u32 n = str.find_last_not_of("\t\n\x0b\x0c\r ");
			_Tstring right(n == _Tstring::npos ? str : str.substr(0, n + 1));
			//_Tstring right = boost::trim_right_copy(str_);
			return right;
		}

		static i32 split(const StringTType &str, std::vector<StringTType> &tokens, const _Tchar *delim)
		{
			const _Tstring delimstr(delim);
			return split(str, tokens, delimstr);
		}
		static i32 split(const StringTType &str, std::vector<StringTType> &tokens, char delim)
		{
			_Tstring delimstr;
			delimstr.push_back(delim);
			return split(str, tokens, delimstr);
		}
		static i32 split(const StringTType &str, std::vector<StringTType> &tokens, const _Tstring &delim)
		{
			std::vector<_Tstring> tokenlist;
			i32 result = split(str, tokenlist, delim);

			tokens.clear();
			typename std::vector<_Tstring>::iterator it;
			for(it = tokenlist.begin() ; it != tokenlist.end() ; it++)
			{
				const _Tstring &token = *it;
				tokens.push_back(StringTType(token));
			}
			return result;
		}
		static i32 split(const _Tstring &str, std::vector<_Tstring> &tokens, const _Tstring &delim)
		{
			tokens.clear();
			
			//if no delimeter
			if(delim.length() == 0)
			{
				tokens.push_back(str);
				return -1;
			}
			if(str.length() == 0)
			{
				return 0;
			}

			//simple impl
			_Tstring token;
			for(u32 i = 0 ; i < str.length() ; i++)
			{
				const _Tchar ch = str[i];
				size_t found = delim.find_first_of(ch);
				if(found == _Tstring::npos)
				{
					//token get!
					token.push_back(ch);
				}
				else
				{
					if(token.length() > 0)
					{
						tokens.push_back(token);
					}
					token.clear();
				}
			}
			//reminder
			if(token.length() > 0)
			{
				tokens.push_back(token);
			}
			return tokens.size();
		}
		static i32 split(const StringTType &str, std::vector<StringTType> &tokens, const StringTType &delim)
		{
			const _Tstring delimstr(delim);
			return splitDelim(str, tokens, delimstr);
		}

		static i32 tokenize(const StringTType &str, std::vector<StringTType> &tokens, const _Tchar *delim)
		{
			const _Tstring delimstr(delim);
			return tokenize(str, tokens, delimstr);
		}
		static i32 tokenize(const _Tstring &str, std::vector<_Tstring> &tokens, const _Tstring &delim)
		{
			tokens.clear();
			if(delim.size() == 0)
			{
				tokens.push_back(str);
				return -1;
			}
			if(str.size() == 0)
			{
				return 0;
			}
			//logit start
			size_t start = 0;
			while(true)
			{
				size_t found = str.find(delim, start);
				if(found == _Tstring::npos)
				{
					if(start < str.length())
					{
						_Tstring tmp = str.substr(start);
						tokens.push_back(tmp);
					}
					return tokens.size();
				}
				else
				{
					_Tstring tmp = str.substr(start, found-start);
					tokens.push_back(tmp);
					
					start = found + delim.size();
				}
			}
			//do not reach
			MT_ASSERT(false && "do not reach");

		}
		static i32 tokenize(const StringTType &str, std::vector<StringTType> &tokens, const _Tstring &delim)
		{
			const _Tstring &strtmp = str;
			std::vector<_Tstring> tokenlist;
			i32 result = tokenize(strtmp, tokenlist, delim);
			
			tokens.clear();
			typename std::vector<_Tstring>::iterator it;
			for(it = tokenlist.begin() ; it != tokenlist.end() ; it++)
			{
				const _Tstring &token = *it;
				tokens.push_back(StringTType(token));
			}
			return result;
		}
		static i32 tokenize(const StringTType &str, std::vector<StringTType> &tokens, const StringTType &delim)
		{
			const _Tstring &delimstr = static_cast<_Tchar&>(delim);
			return tokenize(str, tokens, delimstr); 
		}


		static StringTType join(const StringTType &str, const std::vector<StringTType> &tokens)
		{
			std::vector<_Tstring> tmptokens;
			for(std::size_t i = 0 ; i < tokens.size() ; i++)
			{
				const StringTType &token = tokens[i];
				tmptokens.push_back(static_cast<const _Tstring&>(token));
			}
			
			return join(str, tmptokens);
		}

		static StringTType join(const StringTType &str, const std::vector<_Tstring> &tokens)
		{
			_Tstring result;
			int tokenCnt = tokens.size();
			for(int i = 0 ; i < tokenCnt ; i++)
			{
				result += tokens[i];
				if(i == tokenCnt-1)
				{
					break;
				}
				result += static_cast<const _Tstring&>(str);
			}
			return result;
		}
		
	};
}

namespace matsu
{
	typedef StringTUtil<std::string, char> StringUtil;
	typedef StringTUtil<std::wstring, wchar_t> WStringUtil;
}

