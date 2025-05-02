// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "BTDecorator_DoesActorHaveTag.generated.h"

UCLASS()
class PROJECTA_API UBTDecorator_DoesActorHaveTag : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_DoesActorHaveTag();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Custom | Decorator")
	FBlackboardKeySelector InActorKeyToCheck;

	UPROPERTY(EditAnywhere, Category = "Custom | Decorator")
	FGameplayTag TagToCheck;

	// �±װ� ���� ��츦 �Ǵ��ؾ� �� ���� �̸� true�� ����
	UPROPERTY(EditAnywhere, Category = "Custom | Decorator")
	bool InverseConditionCheck;
};
