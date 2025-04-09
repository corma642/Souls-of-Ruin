// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"

#include "Characters/PA_CharacterPlayer.h"
#include "Controllers/PA_PlayerController.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"

#include "PA_GameplayTags.h"

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

FGameplayEffectSpecHandle UPA_PlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InBaseWeaponDamage, FGameplayTag InCurrentAttackTypeTag, int32 InComboAttackCount)
{
	// 게임플레이 이펙트 콘텍스트 핸들 생성
	FGameplayEffectContextHandle ContextHandle = GetPAAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	// 어빌리티 설정 및 원인 객체, Instigator 설정
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 게임플레이 이펙트 콘텍스트 핸들 생성
	FGameplayEffectSpecHandle Spec = GetPAAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, 1, ContextHandle);

	// 기본 대미지 태그에 무기 기본 피해량 저장
	Spec.Data->SetSetByCallerMagnitude(PA_GameplayTags::Shared_SetByCaller_BaseDamage, InBaseWeaponDamage);

	// 현재 공격에 유형 태그가 있는 경우
	if (InCurrentAttackTypeTag.IsValid())
	{
		// 현재 공격 유형 태그에 콤보 수 저장
		Spec.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InBaseWeaponDamage);
	}

	return Spec;
}
