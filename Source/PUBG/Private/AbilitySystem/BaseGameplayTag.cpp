

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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Turn, "InputTag.Turn");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Punch, "InputTag.Punch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Inventory, "InputTag.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interaction, "InputTag.Interaction");

	// Weapon Input Tag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Equip, "InputTag.Weapon.Equip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_UnEquip, "InputTag.Weapon.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Fire, "InputTag.Weapon.Fire");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Reload, "InputTag.Weapon.Reload");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_ChangeFireMode, "InputTag.Weapon.ChangeMode");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Zoom, "InputTag.Weapon.Zoom");

	// Weapon Action Tag
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Equip, "Player.Ability.Weapon.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_UnEquip, "Player.Ability.Weapon.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Fire, "Player.Ability.Weapon.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Reload, "Player.Ability.Weapon.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_ChangeFireMode, "Player.Ability.Weapon.ChangeFireMode");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Zoom, "Player.Ability.Weapon.Zoom");

	// Weapon Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_Equip, "Player.Event.Action.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_UnEquip, "Player.Event.Action.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_Reload, "Player.Event.Action.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_ReloadEnd, "Player.Event.Action.ReloadEnd");

	UE_DEFINE_GAMEPLAY_TAG(Player_State_Prone, "Player.State.Prone");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Stand, "Player.State.Stand");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Crouch, "Player.State.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Turn, "Player.Ability.Turn");
	
	// Data Tag
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
}
