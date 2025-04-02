// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_EnemyGameplayAbility.h"
#include "Characters/PA_CharacterEnemy.h"

APA_CharacterEnemy* UPA_EnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = Cast<APA_CharacterEnemy>(CurrentActorInfo->AvatarActor);
	}

	return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UPA_EnemyCombatComponent* UPA_EnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	if (APA_CharacterEnemy* EnemyCharacter = GetEnemyCharacterFromActorInfo())
	{
		return EnemyCharacter->GetEnemyCombatComponent();
	}
	return nullptr;
}
