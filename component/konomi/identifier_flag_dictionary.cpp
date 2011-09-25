// Ŭnicode please
#include "precompile.h"
#include "konomi/identifier_flag_dictionary.h"

using namespace std;

namespace konomi
{
	const IdentifierFlagDictionary::IdentifierType &IdentifierFlagDictionary::GetIdentifier(FlagType flag) const
	{
		FlagDictType::const_iterator it = flag_dict_.begin();
		FlagDictType::const_iterator endit = flag_dict_.end();
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
	IdentifierFlagDictionary::FlagType IdentifierFlagDictionary::GetFlag(const IdentifierType &identifier) const
	{
		FlagDictType::const_iterator found = flag_dict_.find(identifier);
		if(found == flag_dict_.end()) 
		{
			return 0;	//cannot find
		}
		else
		{
			return found->second;
		}
	}
}