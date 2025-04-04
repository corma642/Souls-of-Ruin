// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "PA_PlayerGameplayAbility.generated.h"

UCLASS()
class PROJECTA_API UPA_PlayerGameplayAbility : public UPA_GameplayAbility
{
	GENERATED_BODY()

public:
	// 플레이어 캐릭터 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class APA_CharacterPlayer* GetPlayerCharacterFromActorInfo();

	// 플레이어 컨트롤러 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class APA_PlayerController* GetPlayerControllerFromActorInfo();

	// 플레이어 전투 컴포넌트 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class UPA_PlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

private:
	// 플레이어 캐릭터 캐시 데이터
	TWeakObjectPtr<class APA_CharacterPlayer> CachedPlayerCharacter;

	// 플레이어 컨트롤러 캐시 데이터
	TWeakObjectPtr<class APA_PlayerController> CachedPlayerController;


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	// 적에게 적용시킬 게임플레이 이펙트 스펙 핸들 생성 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(
		TSubclassOf<UGameplayEffect> EffectClass,
		float InBaseWeaponDamage,
		FGameplayTag InCurrentAttackTypeTag,
		int32 InComboAttackCount
	);
};
