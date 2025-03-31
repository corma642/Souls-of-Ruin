// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PlayerCombatComponent.h"
#include "Items/Weapons/PA_PlayerWeapon.h"

APA_PlayerWeapon* UPA_PlayerCombatComponent::GetPlayerCarriedByTag(FGameplayTag InWeaponTagToGet) const
{
	return Cast<APA_PlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTagToGet));
}
