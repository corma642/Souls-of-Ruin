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
	// Ÿ�� ������ Ű
	UPROPERTY(EditAnywhere, Category = "Custom | Service")
	FBlackboardKeySelector InTargetActorKey;

	// ���� �Ÿ��� ���� ������ Ű
	UPROPERTY(EditAnywhere, Category = "Custom | Service")
	FBlackboardKeySelector OutDistToTargtKey;
};
