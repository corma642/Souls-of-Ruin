// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PA_PawnUIComponent.h"
#include "PA_PlayerUIComponent.generated.h"

UCLASS()
class PROJECTA_API UPA_PlayerUIComponent : public UPA_PawnUIComponent
{
	GENERATED_BODY()

public:
	// 현재 스태미나 변경 델리게이트 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangeDelegate OnCurrentStaminaChanged;
};
