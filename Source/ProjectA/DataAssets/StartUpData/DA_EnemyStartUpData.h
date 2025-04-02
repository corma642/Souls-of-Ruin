// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DA_BaseStartUpData.h"
#include "DA_EnemyStartUpData.generated.h"

UCLASS()
class PROJECTA_API UDA_EnemyStartUpData : public UDA_BaseStartUpData
{
	GENERATED_BODY()
	
public:
	// �����Ƽ �ο� �Լ�
	virtual void GiveToAbilitySystemComponent(class UPA_AbilitySystemComponent* InASCToGive) override;

protected:
	// �� ���� �����Ƽ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UPA_EnemyGameplayAbility>> EnemyCombatAbilities;
};
