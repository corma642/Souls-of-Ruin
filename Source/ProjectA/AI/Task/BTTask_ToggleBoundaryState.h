// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Task/BTTask_BaseEnemy.h"
#include "BTTask_ToggleBoundaryState.generated.h"

UCLASS()
class PROJECTA_API UBTTask_ToggleBoundaryState : public UBTTask_BaseEnemy
{
	GENERATED_BODY()
	
public:
	UBTTask_ToggleBoundaryState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 태스크 실행 함수 재정의
	virtual void OnExecuteTask() override;

private:
	// 경계 상태 여부
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bShouldEnable;

	// 경계 상태 최대 이동속도 변경 여부
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bShouldChangeMaxWalkSpeed;

	// 경계 상태 최대 이동속도
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	float BoundaryWalkSpeed;

	// 기본 최대 이동속도 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FBlackboardKeySelector InOriginalMaxWalkSpeedKey;

	// 적 캐릭터 무브먼트 컴포넌트 캐시
	TWeakObjectPtr<class UCharacterMovementComponent> CachedMovementComp;
};
