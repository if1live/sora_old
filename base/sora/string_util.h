// Ŭnicode please
#pragma once

#include "sora/macro.h"
#include <vector>
#include <string>

namespace sora
{
class StringUtil {
public:
  static std::string Trim(const std::string &str);
  static std::string LeftTrim(const std::string &str);
  static std::string RightTrim(const std::string &str);

  static int Split(
    const std::string &str,
    char ch, std::vector<std::string> *retval
    );
    
  static std::string Join(
    const std::string &str,
    const std::vector<std::string> &tokenlist
    );
};
}
/*
///@brief std::string, std::wstring operator collection class
template<typename _Tstring, typename _Tchar>
class StringTUtil {
public:
	typedef StringT<_Tstring, _Tchar> StringTType;


	static int split(const StringTType &str, std::vector<StringTType> &tokens, const _Tchar *delim)
	{
		const _Tstring delimstr(delim);
		return split(str, tokens, delimstr);
	}
	static int split(const StringTType &str, std::vector<StringTType> &tokens, char delim)
	{
		_Tstring delimstr;
		delimstr.push_back(delim);
		return split(str, tokens, delimstr);
	}
	static int split(const StringTType &str, std::vector<StringTType> &tokens, const _Tstring &delim)
	{
		std::vector<_Tstring> tokenlist;
		int result = split(str, tokenlist, delim);

		tokens.clear();
		typename std::vector<_Tstring>::iterator it;
		for(it = tokenlist.begin() ; it != tokenlist.end() ; it++)
		{
			const _Tstring &token = *it;
			tokens.push_back(StringTType(token));
		}
		return result;
	}
	static int split(const _Tstring &str, std::vector<_Tstring> &tokens, const _Tstring &delim)
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
		for(size_t i = 0 ; i < str.length() ; i++)
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
	static int split(const StringTType &str, std::vector<StringTType> &tokens, const StringTType &delim)
	{
		const _Tstring delimstr(delim);
		return splitDelim(str, tokens, delimstr);
	}

	static int tokenize(const StringTType &str, std::vector<StringTType> &tokens, const _Tchar *delim)
	{
		const _Tstring delimstr(delim);
		return tokenize(str, tokens, delimstr);
	}
	static int tokenize(const _Tstring &str, std::vector<_Tstring> &tokens, const _Tstring &delim)
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
		SR_ASSERT(false && "do not reach");

	}
	static int tokenize(const StringTType &str, std::vector<StringTType> &tokens, const _Tstring &delim)
	{
		const _Tstring &strtmp = str;
		std::vector<_Tstring> tokenlist;
		int result = tokenize(strtmp, tokenlist, delim);
			
		tokens.clear();
		typename std::vector<_Tstring>::iterator it;
		for(it = tokenlist.begin() ; it != tokenlist.end() ; it++)
		{
			const _Tstring &token = *it;
			tokens.push_back(StringTType(token));
		}
		return result;
	}
	static int tokenize(const StringTType &str, std::vector<StringTType> &tokens, const StringTType &delim)
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
*/
