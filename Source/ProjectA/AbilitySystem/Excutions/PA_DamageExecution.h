// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PA_DamageExecution.generated.h"

UCLASS()
class PROJECTA_API UPA_DamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UPA_DamageExecution();

	// 계산 함수 재정의
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
