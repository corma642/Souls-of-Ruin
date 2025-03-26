// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PA_GameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnTriggered,	// 입력 또는 이벤트를 통해 트리거하는 경우
	OnGiven,		// 어빌리티를 부여받은 직후 / 일회성
};

UCLASS()
class PROJECTA_API UPA_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 효과가 끝나면 제거되는 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	// 효과가 끝나면 제거되는 이펙트 핸들
	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

	// 시작 후 계속 적용되는 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyOnStart;

protected:
	// 폰 전투 컴포넌트 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class UPA_PawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	// 어빌리티 시스템 컴포넌트 가져오기
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class UPA_AbilitySystemComponent* GetPAAbilitySystemComponentFromActorInfo() const;

protected:
	// 어빌리티 활성화 정책
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability")
	EAbilityActivationPolicy AbilityActivationPolicy = EAbilityActivationPolicy::OnTriggered;
};
