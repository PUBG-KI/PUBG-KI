

#include "AbilitySystem/BaseGameplayTag.h"


namespace BaseGameplayTag
{	
	//Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Walk, "InputTag.Walk");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Prone, "InputTag.Prone");

	UE_DEFINE_GAMEPLAY_TAG(Player_State_Prone, "Player.State.Prone");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Stand, "Player.State.Stand");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Crouch, "Player.State.Crouch");
}
