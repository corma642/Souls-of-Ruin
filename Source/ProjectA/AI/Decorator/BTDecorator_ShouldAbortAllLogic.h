// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ShouldAbortAllLogic.generated.h"

UCLASS()
class PROJECTA_API UBTDecorator_ShouldAbortAllLogic : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ShouldAbortAllLogic();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Custom | Decorator")
	FBlackboardKeySelector InTargetActorKey;
};
