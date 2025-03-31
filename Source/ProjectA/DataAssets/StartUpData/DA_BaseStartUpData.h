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
	// 어빌리티 부여 함수
	void GrantAbilities(const TArray<TSubclassOf<class UPA_GameplayAbility>>& InAbilities, class UPA_AbilitySystemComponent* InASCToGive);

	// 시작 시 부여 및 활성화(OnGive) 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UPA_GameplayAbility>> ActivateOnGiveAbilities;

	// 시작 시 부여(OnTriggered) 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UPA_GameplayAbility>> ReactiveAbilities;

	// 시작 시 적용 게임플레이 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom | StartUpData")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpGameplayEffects;
};
