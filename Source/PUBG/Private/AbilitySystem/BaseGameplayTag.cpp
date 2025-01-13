

#include "AbilitySystem/BaseGameplayTag.h"


namespace BaseGameplayTag
{	
	//Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");

	//Player State
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Crouch, "Player.Status.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Prone, "Player.Status.Prone");
	
}
