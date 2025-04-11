// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PA_PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangeDelegate, float, NewPercent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTA_API UPA_PawnUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPA_PawnUIComponent();

	// 현재 체력 변경 델리게이트 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangeDelegate OnCurrentHealthChanged;
};
