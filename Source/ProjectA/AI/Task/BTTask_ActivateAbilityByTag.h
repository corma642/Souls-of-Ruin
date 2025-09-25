// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbilityByTag.generated.h"

// 태스크 상태 저장 메모리 구조체
struct FActivateAbilityByTagTaskMemory
{
	// 현재 보간 목표 위치
	FVector CurrentWarpLocation;

	// 초기화 확인 플래그
	bool bIsInitialized;
};

UCLASS()
class PROJECTA_API UBTTask_ActivateAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateAbilityByTag();

	// Task 실행 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Task 틱 함수
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// 위치 워프 타겟을 가져오는 함수
	FVector GetLocationWarpTarget();

	// 회전 워프 타겟을 가져오는 함수
	FVector GetRotationWarpTarget();

	// 인스턴스의 메모리를 반환하는 함수
	virtual uint16 GetInstanceMemorySize() const override;

	// 태스크 종료 함수
	void FinishTask(UBehaviorTreeComponent& OwnerComp, bool bIsSucceeded);

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

	// 모션 워핑 애니메이션 속도
	UPROPERTY(EditAnywhere, Category = "Custom | Task", meta = (EditCondition = "bUseMotionWarping"))
	float bMotionWarpingSpeed;

	// 공격 중 피해를 받았을 때 공격이 중단될 수 있는지 여부
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bIsCanStopAttack;

	// 캐릭터 캐시
	class APA_CharacterEnemy* AICharacter = nullptr;

	// 공격 대상 캐시
	class AActor* TargetActor = nullptr;
};
