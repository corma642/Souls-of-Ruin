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
	// �����Ƽ �ο� �Լ�
	virtual void GiveToAbilitySystemComponent(class UPA_AbilitySystemComponent* InASCToGive) override;

protected:
	// �÷��̾� ���� �����Ƽ ��Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<FPlayerWeaponAbilitySet> PlayerStartUpAbiltySets;
};
