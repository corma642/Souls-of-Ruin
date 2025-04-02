
#include "PA_GameplayTags.h"

namespace PA_GameplayTags
{
	/* Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraZoom, "InputTag.CameraZoom");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_DualWield, "InputTag.Equip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnEquip_DualWield, "InputTag.UnEquip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Light_DualWield, "InputTag.Attack.Light.DualWield");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable, "InputTag.Toggleable");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld, "InputTag.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Sprint, "InputTag.MustBeHeld.Sprint");


	/* Player Tags */
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sprint, "Player.Ability.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_DualWield, "Player.Ability.Equip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_UnEquip_DualWield, "Player.Ability.UnEquip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_DualWield, "Player.Ability.Attack.Light.DualWield");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_DualWield, "Player.Event.Equip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnEquip_DualWield, "Player.Event.UnEquip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_NextComboAttack, "Player.Event.NextComboAttack");

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_DualWield, "Player.Weapon.DualWield");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Sprinting, "Player.Status.Sprinting");


	/* Enemy Tags */
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Goblin, "Enemy.Weapon.Goblin");
}
