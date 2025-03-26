// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Controllers/PA_PlayerController.h"

APA_CharacterPlayer* UPA_PlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	// 플레이어 캐릭터를 가져올 때 마다, 캐스팅하는 비용을 줄이기 위해 캐시 데이터 활용
	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<APA_CharacterPlayer>(CurrentActorInfo->AvatarActor);
	}
	return CachedPlayerCharacter.IsValid() ? CachedPlayerCharacter.Get() : nullptr;
}

APA_PlayerController* UPA_PlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	// 플레이어 컨트롤러를 가져올 때 마다, 캐스팅하는 비용을 줄이기 위해 캐시 데이터 활용
	if (!CachedPlayerController.IsValid())
	{
		CachedPlayerController = Cast<APA_PlayerController>(CurrentActorInfo->PlayerController);
	}
	return CachedPlayerController.IsValid() ? CachedPlayerController.Get() : nullptr;
}

UPA_PlayerCombatComponent* UPA_PlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}
