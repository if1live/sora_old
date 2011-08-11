// Ŭnicode please
#pragma once

namespace konomi
{
	///@brief many string->many flag converter
	///for example, you have string array[key1, key2].
	///you want to use string element as flag
	///such as,,key1=0x01, key2=0x02
	///this class will do these work
	class IdentifierFlagDictionary {
	public:
		typedef unsigned int FlagType;
		typedef std::string IdentifierType;
		typedef std::tr1::unordered_map<IdentifierType, FlagType> FlagDictType;
	public:
		template<typename T>
		IdentifierFlagDictionary(const T &strlist);

		const IdentifierType &getIdentifier(FlagType flag) const;

		///@return flag value if identifier exist,
		///0 if identifier doesn't not exist
		FlagType getFlag(const IdentifierType &identifier) const;

		///@return found identifier count
		template<typename T>
		int getIdentifiers(FlagType flag, T &result) const;
	private:
		FlagDictType flagDict_;
	};
}

namespace konomi
{
	template<typename T>
	IdentifierFlagDictionary::IdentifierFlagDictionary(const T &strlist)
	{
		using namespace std;
		typename T::const_iterator it = strlist.begin();
		typename T::const_iterator endit = strlist.end();
		for(FlagType i = 0x01 ; it != endit ; it++, i <<= 1)
		{
			const IdentifierType &str = *it;
			flagDict_[str] = i;
		}
	}

	template<typename T>
	int IdentifierFlagDictionary::getIdentifiers(FlagType flag, T &result) const
	{
		using namespace std;
		int flagCount = flagDict_.size();
		int identifierCnt = 0;
		for(FlagType currFlag = 1, i = 0 ; i < flagCount ; i++, currFlag <<= 1)
		{
			if((flag & currFlag) != currFlag)
			{
				continue;
			}
			const IdentifierType &identifier = getIdentifier(currFlag);
			if(identifier.empty() == false)
			{
				result.push_back(identifier);
				identifierCnt++;
			}
		}
		return identifierCnt;
	}
}