// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PA_GameplayAbility.generated.h"

UCLASS()
class PROJECTA_API UPA_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 플레이어 캐릭터 가져오기
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class APA_CharacterPlayer* GetPlayerCharacterFromActorInfo() const;

	// 적 캐릭터 가져오기
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class APA_CharacterEnemy* GetEnemyCharacterFromActorInfo() const;

	// 효과가 끝나면 제거되는 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	// 효과가 끝나면 제거되는 이펙트 핸들
	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

	// 시작 후 계속 적용되는 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyOnStart;
};
