// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Stats/PBStatDefinition.h"

#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"


bool UPBStatDefinition::GetStatEntryForLevel(int Level, FPBStatLevelEntry& OutEntry)
{
	if(StatEntries.IsValidIndex(Level-1))
	{
		OutEntry = StatEntries[Level - 1];
		return true;
	}

	UE_LOGFMT(LogPBStats, Error, "UPBStatDefinition::GetStatEntryForLevel(). StatDef {0} Does not have entry for level: {1}", StatName.ToString(), Level);

	return false;
}
