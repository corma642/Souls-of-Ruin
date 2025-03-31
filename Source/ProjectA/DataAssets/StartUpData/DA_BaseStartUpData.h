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
	virtual void GiveToAbilitySystemComponent(class UPA_AbilitySystemComponent* InASCToGive);

protected:
	// �����Ƽ �ο� �Լ�
	void GrantAbilities(const TArray<TSubclassOf<class UPA_GameplayAbility>>& InAbilities, class UPA_AbilitySystemComponent* InASCToGive);

	// ���� �� �ο� �� Ȱ��ȭ(OnGive) �����Ƽ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UPA_GameplayAbility>> ActivateOnGiveAbilities;

	// ���� �� �ο�(OnTriggered) �����Ƽ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UPA_GameplayAbility>> ReactiveAbilities;

	// ���� �� ���� �����÷��� ����Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpGameplayEffects;
};
