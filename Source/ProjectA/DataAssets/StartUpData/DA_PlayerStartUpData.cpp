// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DA_PlayerStartUpData.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"

void UDA_PlayerStartUpData::GiveToAbilitySystemComponent(UPA_AbilitySystemComponent* InASCToGive)
{
	Super::GiveToAbilitySystemComponent(InASCToGive);

	for (const FPlayerWeaponAbilitySet& Ability : PlayerStartUpAbiltySets)
	{
		if (!Ability.IsValid()) continue;
		{
			FGameplayAbilitySpec AbilitySpec(Ability.AbilityToGrant.Get());
			AbilitySpec.SourceObject = this;
			AbilitySpec.DynamicAbilityTags.AddTag(Ability.InputTag);

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
