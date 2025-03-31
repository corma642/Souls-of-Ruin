// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DA_InputConfig.generated.h"

// 입력 액션 구성
USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY()

public:
	// 입력 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "InputTag"))
	FGameplayTag InputTag;

	// 입력 액션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> InputAction;

	bool IsValid() const { return InputAction && InputTag.IsValid(); }
};

UCLASS()
class PROJECTA_API UDA_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// 입력 태그를 통해, 기본 입력 액션 중에 해당되는 입력 액션 반환
	class UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// 기본 입력 액션 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> NativeInputActions;

	// 어빌리티 입력 액션 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> AbilityInputActions;
};
