// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "GA_Player_MotionWarping.generated.h"

UCLASS()
class PROJECTA_API UGA_Player_MotionWarping : public UPA_PlayerGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Player_MotionWarping();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// 몽타주 재생 함수
	void MontageToPlay();

	// 모션 워핑 방향 및 거리 설정 함수
	void ComputeDirectionAndDistance();

protected:
	// 모션 워핑 사용 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	bool bUseMotionWarping;

	// 몽타주 플레이 태스크
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	// 재생할 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TObjectPtr<class UAnimMontage> PlayMontage;

	// 바닥(착지 가능) 오브젝트 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> GroundObjectTypes;

	// 이동 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float MoveDistance = 500.f;
};
