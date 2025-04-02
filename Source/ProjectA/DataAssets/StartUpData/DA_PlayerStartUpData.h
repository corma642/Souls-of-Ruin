// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DA_BaseStartUpData.h"
#include "ProjectATypes/PA_StructTypes.h"
#include "DA_PlayerStartUpData.generated.h"

UCLASS()
class PROJECTA_API UDA_PlayerStartUpData : public UDA_BaseStartUpData
{
	GENERATED_BODY()

public:
	// 어빌리티 부여 함수
	virtual void GiveToAbilitySystemComponent(class UPA_AbilitySystemComponent* InASCToGive) override;

protected:
	// 플레이어 시작 어빌리티 세트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<FPlayerWeaponAbilitySet> PlayerStartUpAbiltySets;
};
