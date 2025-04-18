// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookAtTarget.generated.h"

UCLASS()
class PROJECTA_API UBTService_LookAtTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_LookAtTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// 타깃 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Cusstom | Service")
	FBlackboardKeySelector InTargetActorKey;

	// 회전 시, 회전 보간 값
	UPROPERTY(EditAnywhere, Category = "Cusstom | Service")
	float RotationInterpSpeed;
};
