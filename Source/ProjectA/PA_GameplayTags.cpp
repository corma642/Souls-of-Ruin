
#include "PA_GameplayTags.h"

namespace PA_GameplayTags
{
	/* Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraZoom, "InputTag.CameraZoom");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_DualWield, "InputTag.Equip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnEquip_DualWield, "InputTag.UnEquip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Light_DualWield, "InputTag.Attack.Light.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_SprintAttack_DualWield, "InputTag.Attack.SprintAttack.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ChangeLockOnTarget, "InputTag.ChangeLockOnTarget");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable, "InputTag.Toggleable");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_TargetLockOn, "InputTag.Toggleable.TargetLockOn");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld, "InputTag.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Sprint, "InputTag.MustBeHeld.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Block, "InputTag.MustBeHeld.Block");


	/* Player Tags */
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sprint, "Player.Ability.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_DualWield, "Player.Ability.Equip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_UnEquip_DualWield, "Player.Ability.UnEquip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_DualWield, "Player.Ability.Attack.Light.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_SprintAttack_DualWield, "Player.Ability.Attack.SprintAttack.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, "Player.Ability.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "Player.Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, "Player.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLockOn, "Player.Ability.TargetLockOn");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_DualWield, "Player.Event.Equip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnEquip_DualWield, "Player.Event.UnEquip.DualWield");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_NextComboAttack, "Player.Event.NextComboAttack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SuccessfulBlock, "Player.Event.SuccessfulBlock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ChangeLockOnTarget_Left, "Player.Event.ChangeLockOnTarget.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ChangeLockOnTarget_Right, "Player.Event.ChangeLockOnTarget.Right");

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_DualWield, "Player.Weapon.DualWield");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Attacking, "Player.Status.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Sprinting, "Player.Status.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling, "Player.Status.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Blocking, "Player.Status.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLockOn, "Player.Status.TargetLockOn");

	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light");


	/* Enemy Tags */
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability, "Enemy.Ability");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_MeleeAttack, "Enemy.Ability.MeleeAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_RangeAttack, "Enemy.Ability.RangeAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SpactialAttack, "Enemy.Ability.SpactialAttack");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Goblin, "Enemy.Weapon.Goblin");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Boundary, "Enemy.Status.Boundary");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnBlockable, "Enemy.Status.UnBlockable");


	/* Shared Tags */
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ThrowProjectile, "Shared.Event.ThrowProjectile");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");
}
