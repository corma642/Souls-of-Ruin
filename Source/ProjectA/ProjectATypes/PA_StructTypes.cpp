// Vince Petrell All Rights Reserved


#include "ProjectATypes/PA_StructTypes.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"

bool FPlayerWeaponAbilitySet::IsValid() const
{
    return AbilityToGrant && InputTag.IsValid();
}
