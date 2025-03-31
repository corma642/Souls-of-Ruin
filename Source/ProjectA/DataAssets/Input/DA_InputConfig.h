// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DA_InputConfig.generated.h"

// �Է� �׼� ����
USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY()

public:
	// �Է� �±�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "InputTag"))
	FGameplayTag InputTag;

	// �Է� �׼�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> InputAction;

	bool IsValid() const { return InputAction && InputTag.IsValid(); }
};

UCLASS()
class PROJECTA_API UDA_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// �Է� �±׸� ����, �⺻ �Է� �׼� �߿� �ش�Ǵ� �Է� �׼� ��ȯ
	class UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// �⺻ �Է� �׼� �迭
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> NativeInputActions;

	// �����Ƽ �Է� �׼� �迭
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> AbilityInputActions;
};
