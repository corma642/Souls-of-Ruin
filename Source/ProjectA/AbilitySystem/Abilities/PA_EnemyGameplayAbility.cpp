// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_EnemyGameplayAbility.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"

#include "PA_GameplayTags.h"

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

FGameplayEffectSpecHandle UPA_EnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InDamage)
{
	// �����÷��� ����Ʈ ���ؽ�Ʈ �ڵ� ����
	FGameplayEffectContextHandle ContextHandle = GetPAAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	// �����Ƽ ���� �� ���� ��ü, Instigator ����
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// �����÷��� ����Ʈ ���ؽ�Ʈ �ڵ� ����
	FGameplayEffectSpecHandle Spec = GetPAAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, 1, ContextHandle);

	// �⺻ ����� �±׿� ���ط� ����
	Spec.Data->SetSetByCallerMagnitude(PA_GameplayTags::Shared_SetByCaller_BaseDamage, InDamage);

	return Spec;
}
