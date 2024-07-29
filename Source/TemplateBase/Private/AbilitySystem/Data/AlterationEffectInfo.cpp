// Retropsis @ 2024

#include "AbilitySystem/Data/AlterationEffectInfo.h"
#include "TemplateBase/LogChannels.h"

FAlterationEffect UAlterationEffectInfo::FindAlterationEffectForTag(const FGameplayTag& AlterationEffectTag, bool bLogNotFound) const
{
	for (const FAlterationEffect& Info : AlterationEffectInformation)
	{
		if(Info.AlterationEffectTag == AlterationEffectTag)
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogBase, Error, TEXT("Can't find Info for AlterationEffectTag [%s] in AlterationEffectInformation [%s]"), *AlterationEffectTag.ToString(), *GetNameSafe(this))
	}
	return FAlterationEffect();
}
