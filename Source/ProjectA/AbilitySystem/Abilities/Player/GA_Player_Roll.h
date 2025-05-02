// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "GA_Player_Roll.generated.h"

UCLASS()
class PROJECTA_API UGA_Player_Roll : public UPA_PlayerGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Player_Roll();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 몽타주 재생 함수
	void PlayRollMontage();

	// 구르기 방향 및 거리 설정 함수
	void ComputeRollDirectionAndDistance();

protected:
	// 몽타주 플레이 태스크
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	// 구르기 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TObjectPtr<class UAnimMontage> RollMontage;

	// 구른 후, 서있을 수 있는 오브젝트 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> RollObjectTypes;

	// 구르기 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float RollDistance = 500.f;

private:
	// 시작 딜레이 타이머 핸들
	FTimerHandle DelayTimerHandle;
};
