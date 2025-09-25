// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PA_PawnUIComponent.h"
#include "PA_EnemyUIComponent.generated.h"

UCLASS()
class PROJECTA_API UPA_EnemyUIComponent : public UPA_PawnUIComponent
{
	GENERATED_BODY()

public:
	UPA_EnemyUIComponent();

	// 적이 그린 위젯을 등록하는 함수
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidgets(class UPA_BaseWidget* InWidgetToRegister);

	// 적이 그린 위젯을 제거하는 함수
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny();

private:
	// 적이 그린 위젯
	TArray<TObjectPtr<class UPA_BaseWidget>> EnemyDrawnWidgets;
};
