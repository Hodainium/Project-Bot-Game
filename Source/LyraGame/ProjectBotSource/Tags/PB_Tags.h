// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace PB_CommonUI_Tags
{
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_ACTION_MAINMENU);

	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_GAME);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_GAMEMENU);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_MENU);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_LAYER_MODAL);
}

namespace PB_GameplayEvent_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GAMEPLAYEVENT_SHIELDBREAK);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GAMEPLAYEVENT_KNOCKBACK);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GAMEPLAYEVENT_DEATH);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GAMEPLAYEVENT_RESET);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GAMEPLAYEVENT_REQUESTRESET);
}

namespace PB_Ability_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ABILITY_BEHAVIOR_SURVIVESDEATH);

}

namespace PB_Status_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_STATUS_CROUCHING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_STATUS_AUTORUNNING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_STATUS_DEATH);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_STATUS_DEATPB_DYING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_STATUS_DEATPB_DEAD);
}

namespace PB_Message_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ELIMINATION_MESSAGE);

	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SHIELDBREAK_MESSAGE);

	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DAMAGE_MESSAGE);
}

namespace PB_SetByCaller_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SETBYCALLER_DAMAGE);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SETBYCALLER_HEAL);
}

namespace PB_Damage_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DAMAGE_SELFDESTRUCT);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DAMAGE_FELLOUTOFWORLD);
}

namespace PB_Status_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DAMAGE_IMMUNITY);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DAMAGE_INTANGIBLE);
}

namespace PB_DamageType_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DAMAGETYPE_EXPLOSION);
}

namespace PB_Cheat_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CHEAT_GODMODE);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CHEAT_UNLIMITEDHEALTH);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CHEAT_UNLIMITEDSTAMINA);
}

namespace PB_Knockback_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_KNOCKBACK_LEVEL_0);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_KNOCKBACK_LEVEL_1);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_KNOCKBACK_LEVEL_2);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_KNOCKBACK_LEVEL_3);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_KNOCKBACK_LEVEL_4);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_KNOCKBACK_LEVEL_5);
}

namespace PB_MovementMode_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_MODE_WALKING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_MODE_NAVWALKING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_MODE_FALLING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_MODE_SWIMMING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_MODE_FLYING);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_MODE_CUSTOM);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	LYRAGAME_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	LYRAGAME_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
}

namespace PB_MovementSetting_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_USECONTROLLERROTATION);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_ORIENTTOMOVEMENT);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_FOLLOWMOVEINPUT);

	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_ALLOWROTATIONDURINGROOTMOTION);


	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_LOCKMOVEMENT);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_LOCKMOVEINPUT);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_LOCKROTATION);

	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_ACCEPTCLIENTPOSITION);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MOVEMENT_SETTING_IGNOREMOVEMENTCHECK);
}

namespace PB_Movement_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GAMEPLAY_MOVEMENTSTOPPED);
}

namespace PB_Inventory_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_INVENTORY_PUSHITEMPROMPT);
}

namespace PB_ItemSlots_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEMSLOTS_MESSAGE_SLOTSCHANGED);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEMSLOTS_MESSAGE_ACTIVEINDEXCHANGED);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEMSLOTS_MESSAGE_NUMSLOTSCHANGED);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEMSLOTS_MESSAGE_TWOHANDINGCHANGED);
}

namespace PB_ItemQuality_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEM_QUALITY_0);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEM_QUALITY_1);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEM_QUALITY_2);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEM_QUALITY_3);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEM_QUALITY_4);
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ITEM_QUALITY_5);
}

namespace PB_Camera_Tags
{
	LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_SteadyAimingCamera);
}