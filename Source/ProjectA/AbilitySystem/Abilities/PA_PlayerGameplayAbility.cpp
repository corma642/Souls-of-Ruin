// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"

#include "Characters/PA_CharacterPlayer.h"
#include "Controllers/PA_PlayerController.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"

#include "PA_GameplayTags.h"

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

FGameplayEffectSpecHandle UPA_PlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InBaseWeaponDamage, FGameplayTag InCurrentAttackTypeTag, int32 InComboAttackCount)
{
	// �����÷��� ����Ʈ ���ؽ�Ʈ �ڵ� ����
	FGameplayEffectContextHandle ContextHandle = GetPAAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	// �����Ƽ ���� �� ���� ��ü, Instigator ����
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// �����÷��� ����Ʈ ���ؽ�Ʈ �ڵ� ����
	FGameplayEffectSpecHandle Spec = GetPAAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, 1, ContextHandle);

	// �⺻ ����� �±׿� ���� �⺻ ���ط� ����
	Spec.Data->SetSetByCallerMagnitude(PA_GameplayTags::Shared_SetByCaller_BaseDamage, InBaseWeaponDamage);

	// ���� ���ݿ� ���� �±װ� �ִ� ���
	if (InCurrentAttackTypeTag.IsValid())
	{
		// ���� ���� ���� �±׿� �޺� �� ����
		Spec.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InBaseWeaponDamage);
	}

	return Spec;
}
