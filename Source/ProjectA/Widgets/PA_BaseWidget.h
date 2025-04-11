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
	// �� ���� �� ���� �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintCallable, Category = "Custom | Widget")
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);

protected:
	// ���� ���� �� �� �� ȣ��Ǵ� �Լ�
	virtual void NativeOnInitialized() override;

	// �÷��̾� UI ������Ʈ �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom | Widget", meta = (DisplayName = "On Owing Player UI Component Initialized"))
	void BP_OnOwingPlayerUIComponentInitialized(class UPA_PlayerUIComponent* OwningPlayerUIComponent);

	// �� UI ������Ʈ �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom | Widget", meta = (DisplayName = "On Owing Enemy UI Component Initialized"))
	void BP_OnOwingEnemyUIComponentInitialized(class UPA_EnemyUIComponent* OwningEnemyUIComponent);
};
