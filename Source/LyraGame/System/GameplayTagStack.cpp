// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTagStack.h"

#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "UObject/Stack.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayTagStack)

//////////////////////////////////////////////////////////////////////
// FGameplayTagStack

FString FGameplayTagStack::GetDebugString() const
{
	return FString::Printf(TEXT("%sx%d"), *Tag.ToString(), StackCount);
}

//////////////////////////////////////////////////////////////////////
// FGameplayTagStackContainer

void FGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				MarkItemDirty(Stack);
				OnTagStackChanged.Broadcast(Tag, NewCount);
				return;
			}
		}

		FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);

		OnTagStackChanged.Broadcast(Tag, StackCount);
	}
}

void FGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return;
	}

	//@TODO: Should we error if you try to remove a stack that doesn't exist or has a smaller count?
	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{
					It.RemoveCurrent();
					TagToCountMap.Remove(Tag);
					MarkArrayDirty();
					OnTagStackChanged.Broadcast(Tag, 0);
				}
				else
				{
					const int32 NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
					MarkItemDirty(Stack);
					OnTagStackChanged.Broadcast(Tag, NewCount);
				}

				

				return;
			}
		}
	}
}

FString FGameplayTagStackContainer::GetDebugString() const
{
	FString Output; 

	for (auto Stack : TagToCountMap)
	{
		Output += Stack.Key.ToString();
		Output += FString::Printf(TEXT("x"));
		Output += FString::Printf(TEXT("%d"), Stack.Value);
		Output += FString::Printf(TEXT(",   "));
	}

	/*for (FGameplayTagStack Stack : Stacks)
	{
		Output += Stack.GetDebugString();
		Output += FString::Printf(TEXT(", "));
	}*/
	return Output;
}

FString FGameplayTagStackContainer::GetDebugString2() const
{
	FString Output;

	for (FGameplayTagStack Stack : Stacks)
	{
		Output += Stack.GetDebugString();
		Output += FString::Printf(TEXT(", "));
	}

	return Output;
}

void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = Stacks[Index].Tag;
		OnTagStackChanged.Broadcast(Tag, 0);
		TagToCountMap.Remove(Tag);
	}
}

void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[Index];
		TagToCountMap.Add(Stack.Tag, Stack.StackCount);
		OnTagStackChanged.Broadcast(Stack.Tag, Stack.StackCount);
	}
}

void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[Index];
		TagToCountMap[Stack.Tag] = Stack.StackCount;
		OnTagStackChanged.Broadcast(Stack.Tag, Stack.StackCount);
	}
}

