// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Controllers/PA_PlayerController.h"

APA_CharacterPlayer* UPA_PlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	// �÷��̾� ĳ���͸� ������ �� ����, ĳ�����ϴ� ����� ���̱� ���� ĳ�� ������ Ȱ��
	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<APA_CharacterPlayer>(CurrentActorInfo->AvatarActor);
	}
	return CachedPlayerCharacter.IsValid() ? CachedPlayerCharacter.Get() : nullptr;
}

APA_PlayerController* UPA_PlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	// �÷��̾� ��Ʈ�ѷ��� ������ �� ����, ĳ�����ϴ� ����� ���̱� ���� ĳ�� ������ Ȱ��
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
