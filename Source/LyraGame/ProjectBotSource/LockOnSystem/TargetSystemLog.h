// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

LYRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogPBTargetSystem, Display, All);

#define TS_LOG(Verbosity, Format, ...) \
{ \
    UE_LOG(LogPBTargetSystem, Verbosity, Format, ##__VA_ARGS__); \
}
