// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ComputeAttackChance.generated.h"

UCLASS()
class PROJECTA_API UBTDecorator_ComputeAttackChance : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ComputeAttackChance();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// �ּ� ���� �õ� Ȯ��
	UPROPERTY(EditAnywhere, Category = "Custom | Decorator", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
	float AttackChanceMin;

	// �ִ� ���� �õ� Ȯ��
	UPROPERTY(EditAnywhere, Category = "Custom | Decorator", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
	float AttackChanceMax;
};
