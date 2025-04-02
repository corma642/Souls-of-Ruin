// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DA_EnemyStartUpData.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PA_EnemyGameplayAbility.h"

void UDA_EnemyStartUpData::GiveToAbilitySystemComponent(UPA_AbilitySystemComponent* InASCToGive)
{
	Super::GiveToAbilitySystemComponent(InASCToGive);

	for (const TSubclassOf<UPA_EnemyGameplayAbility>& Ability : EnemyCombatAbilities)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec Spec(Ability);
		Spec.SourceObject = InASCToGive->GetAvatarActor();
		
		InASCToGive->GiveAbility(Spec);
	}
}
