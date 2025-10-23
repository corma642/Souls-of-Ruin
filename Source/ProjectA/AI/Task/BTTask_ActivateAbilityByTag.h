// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbilityByTag.generated.h"

UCLASS()
class PROJECTA_API UBTTask_ActivateAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAbilityByTag();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// 태스크 종료 함수
	void FinishTask(UBehaviorTreeComponent& OwnerComp, bool bIsSucceeded);

	// 콜리전 및 무브먼트 컴포넌트 상태 변경 함수
	void SetUpMotionWarpingState();

	// 콜리전 및 무브먼트 컴포넌트 상태 변경 복구 함수
	void UnSetUpMotionWarpingState();

private:
	// 활성화할 어빌리티 태그
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FGameplayTag AbilityTagToActivate;

	// 공격 중 상태 키
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FBlackboardKeySelector IsAttackingKey;

	// 목표 위치와 떨어질 거리
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	float WarpTargetDistance = 75.0f;

	// 공격 대상 키
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FBlackboardKeySelector TargetActorKey;

	// 모션 워핑 애니메이션 사용 여부
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bUseMotionWarping;

	// Z축 루트 모션 이동이 있는지 여부
	UPROPERTY(EditAnywhere, Category = "Custom | Task", meta = (EditCondition = "bUseMotionWarping"))
	bool bUseZAxisRootMotion;

	// 공격 중 피해를 받았을 때 공격이 중단될 수 있는지 여부
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bIsCanStopAttack;

	// 무시할 콜리전 채널
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelToIgnore;

	// 캐릭터 캐시
	class APA_CharacterEnemy* AICharacter = nullptr;

	// 공격 대상 캐시
	class AActor* TargetActor = nullptr;

private:
	// 변경한 콜리전 채널
	TMap<TEnumAsByte<ECollisionChannel>, ECollisionResponse> ChangedCollisionChannel;

	// 바닥(착지 가능) 오브젝트 타입
	TArray<TEnumAsByte<EObjectTypeQuery>> GroundObjectTypes;
};
