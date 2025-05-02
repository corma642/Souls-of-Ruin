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
	// �����Ƽ �Է� ����(Started) �Լ�
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);

	// �����Ƽ �Է� ����(Completed) �Լ�
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

public:
	// �÷��̾� ���� �����Ƽ �ο�
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability", meta = (ApplyLevel = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerWeaponAbilitySet>& InAbilitiesToGrant);

	// �÷��̾ �ο��� ���� �����Ƽ ����
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability", meta = (ApplyLevel = "1"))
	void RemoveGrantedPlayerWeaponAbilities();

public:
	// �±׸� ���� �����Ƽ Ȱ��ȭ �õ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

private:
	// �ο��� �÷��̾� ���� �����Ƽ
	TArray<FGameplayAbilitySpecHandle> GrantedPlayerWeaponAbilities;
};
