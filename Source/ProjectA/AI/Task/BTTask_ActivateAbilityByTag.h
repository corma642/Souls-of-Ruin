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

	// Task 실행 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Task 틱 함수
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// 위치 워프 타겟을 가져오는 함수
	FVector GetLocationWarpTarget();

	// 회전 워프 타겟을 가져오는 함수
	FVector GetRotationWarpTarget();

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

	// 캐릭터 캐시
	class APA_CharacterEnemy* AICharacter = nullptr;

	// 공격 대상 캐시
	class AActor* TargetActor = nullptr;
};
