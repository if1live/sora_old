// Ŭnicode please
#include "../SoraStdAfx.h"
#include "konomi/KNIdentifierFlagDictionary.h"

using namespace std;

namespace konomi
{
	const IdentifierFlagDictionary::IdentifierType &IdentifierFlagDictionary::getIdentifier(FlagType flag) const
	{
		FlagDictType::const_iterator it = flagDict_.begin();
		FlagDictType::const_iterator endit = flagDict_.end();
		for( ; it != endit ; it++) 
		{
			if(it->second == flag)
			{
				return it->first;
			}
		}
		//else...
		const static IdentifierType empty;
		return empty;
	}
	IdentifierFlagDictionary::FlagType IdentifierFlagDictionary::getFlag(const IdentifierType &identifier) const
	{
		FlagDictType::const_iterator found = flagDict_.find(identifier);
		if(found == flagDict_.end()) 
		{
			return 0;	//cannot find
		}
		else
		{
			return found->second;
		}
	}
}