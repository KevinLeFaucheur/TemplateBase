// Retropsis @ 2024


#include "AbilitySystem/Data/AbilityInfo.h"

#include "TemplateBase/LogChannels.h"

FBaseAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FBaseAbilityInfo& Info : AbilityInformation)
	{
		if(Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogBase, Error, TEXT("Can't find Info for AbilityTag [%s] in AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this))
	}
	return FBaseAbilityInfo();
}
