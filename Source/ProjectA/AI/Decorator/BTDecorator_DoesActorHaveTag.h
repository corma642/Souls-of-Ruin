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

	// 태그가 없는 경우를 판단해야 할 때는 이를 true로 설정
	UPROPERTY(EditAnywhere, Category = "Custom | Decorator")
	bool InverseConditionCheck;
};
