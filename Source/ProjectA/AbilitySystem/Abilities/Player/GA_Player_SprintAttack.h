// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "GA_Player_SprintAttack.generated.h"

UCLASS()
class PROJECTA_API UGA_Player_SprintAttack : public UPA_PlayerGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Player_SprintAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 몽타주 재생 함수
	void PlaySprintAttackMontage();

	// 달리기 공격 이동 거리 설정 함수
	void ComputeAttackDistance();

protected:
	// 몽타주 플레이 태스크
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	// 달리기 공격 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TObjectPtr<class UAnimMontage> SprintAttackMontage;

	// 충돌 오브젝트 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesQuery;

	// 공격 이동 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float AttackDistance = 500.f;
};
