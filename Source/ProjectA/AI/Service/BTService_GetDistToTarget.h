// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetDistToTarget.generated.h"

UCLASS()
class PROJECTA_API UBTService_GetDistToTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_GetDistToTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// 타깃 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Custom | Service")
	FBlackboardKeySelector InTargetActorKey;

	// 구한 거리를 받을 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Custom | Service")
	FBlackboardKeySelector OutDistToTargtKey;
};
