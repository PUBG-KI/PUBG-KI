#pragma once

#include "NativeGameplayTags.h"

namespace BaseGameplayTag
{	
	PUBG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	PUBG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	PUBG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Walk_Ctrl);
	PUBG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint_Shift);

	//Player State
	PUBG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Crouch);
	PUBG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Prone);
}