// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PA_BaseWidget.generated.h"

UCLASS()
class PROJECTA_API UPA_BaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 적 생성 시 위젯 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | Widget")
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);

protected:
	// 위젯 생성 후 한 번 호출되는 함수
	virtual void NativeOnInitialized() override;

	// 플레이어 UI 컴포넌트 초기화 함수
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom | Widget", meta = (DisplayName = "On Owing Player UI Component Initialized"))
	void BP_OnOwingPlayerUIComponentInitialized(class UPA_PlayerUIComponent* OwningPlayerUIComponent);

	// 적 UI 컴포넌트 초기화 함수
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom | Widget", meta = (DisplayName = "On Owing Enemy UI Component Initialized"))
	void BP_OnOwingEnemyUIComponentInitialized(class UPA_EnemyUIComponent* OwningEnemyUIComponent);
};
