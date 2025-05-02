// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "PA_EnemyGameplayAbility.generated.h"

UCLASS()
class PROJECTA_API UPA_EnemyGameplayAbility : public UPA_GameplayAbility
{
	GENERATED_BODY()

public:
	// 적 캐릭터 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class APA_CharacterEnemy* GetEnemyCharacterFromActorInfo();

	// 적 전투 컴포넌트 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class UPA_EnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<class APA_CharacterEnemy> CachedEnemyCharacter;


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	// 플레이어에게 적용시킬 게임플레이 이펙트 스펙 핸들 생성 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(
		TSubclassOf<UGameplayEffect> EffectClass, float InDamage
	);
};
