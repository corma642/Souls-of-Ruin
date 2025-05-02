// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectATypes/PA_StructTypes.h"
#include "InputActionValue.h"
#include "PA_AbilitySystemComponent.generated.h"

UCLASS()
class PROJECTA_API UPA_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 어빌리티 입력 시작(Started) 함수
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);

	// 어빌리티 입력 종료(Completed) 함수
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

public:
	// 플레이어 무기 어빌리티 부여
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability", meta = (ApplyLevel = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerWeaponAbilitySet>& InAbilitiesToGrant);

	// 플레이어에 부여한 무기 어빌리티 제거
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability", meta = (ApplyLevel = "1"))
	void RemoveGrantedPlayerWeaponAbilities();

public:
	// 태그를 통한 어빌리티 활성화 시도 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

private:
	// 부여한 플레이어 무기 어빌리티
	TArray<FGameplayAbilitySpecHandle> GrantedPlayerWeaponAbilities;
};
