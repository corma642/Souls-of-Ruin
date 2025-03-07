// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_BaseStartUpData.generated.h"

UCLASS()
class PROJECTA_API UDA_BaseStartUpData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | AbilitySystem")
	TArray<TSubclassOf<class UGameplayAbility>> StartUpAbilties;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | AbilitySystem")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpEffects;
};
