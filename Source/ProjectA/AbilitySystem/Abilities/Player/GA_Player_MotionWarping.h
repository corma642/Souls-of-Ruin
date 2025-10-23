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
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 몽타주 재생 함수
	void MontageToPlay();

	// 모션 워핑 방향 및 거리 설정 함수
	void ComputeDirectionAndDistance();

	// 콜리전 및 무브먼트 컴포넌트 상태 변경 함수
	void SetUpMotionWarpingState();

	// 콜리전 및 무브먼트 컴포넌트 상태 변경 복구 함수
	void UnSetUpMotionWarpingState();

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

	// 무시할 콜리전 채널
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelToIgnore;

	// 이동 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float MoveDistance = 500.f;

	// 충돌 지점에서 물러날 오프셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float DistanceOffset = 10.f;

private:
	// 변경한 콜리전 채널
	TMap<TEnumAsByte<ECollisionChannel>, ECollisionResponse> ChangedCollisionChannel;
};
