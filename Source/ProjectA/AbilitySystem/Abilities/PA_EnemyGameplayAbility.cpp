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
	// 게임플레이 이펙트 콘텍스트 핸들 생성
	FGameplayEffectContextHandle ContextHandle = GetPAAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	// 어빌리티 설정 및 원인 객체, Instigator 설정
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 게임플레이 이펙트 콘텍스트 핸들 생성
	FGameplayEffectSpecHandle Spec = GetPAAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, 1, ContextHandle);

	// 기본 대미지 태그에 피해량 저장
	Spec.Data->SetSetByCallerMagnitude(PA_GameplayTags::Shared_SetByCaller_BaseDamage, InDamage);

	return Spec;
}
