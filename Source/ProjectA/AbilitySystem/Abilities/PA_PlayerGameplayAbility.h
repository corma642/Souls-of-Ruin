// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "PA_PlayerGameplayAbility.generated.h"

UCLASS()
class PROJECTA_API UPA_PlayerGameplayAbility : public UPA_GameplayAbility
{
	GENERATED_BODY()

public:
	// �÷��̾� ĳ���� ��������
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class APA_CharacterPlayer* GetPlayerCharacterFromActorInfo();

	// �÷��̾� ��Ʈ�ѷ� ��������
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class APA_PlayerController* GetPlayerControllerFromActorInfo();

	// �÷��̾� ���� ������Ʈ ��������
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class UPA_PlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

private:
	// �÷��̾� ĳ���� ĳ�� ������
	TWeakObjectPtr<class APA_CharacterPlayer> CachedPlayerCharacter;

	// �÷��̾� ��Ʈ�ѷ� ĳ�� ������
	TWeakObjectPtr<class APA_PlayerController> CachedPlayerController;


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	// ������ �����ų �����÷��� ����Ʈ ���� �ڵ� ���� �Լ�
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(
		TSubclassOf<UGameplayEffect> EffectClass,
		float InBaseWeaponDamage,
		FGameplayTag InCurrentAttackTypeTag,
		int32 InComboAttackCount
	);
};
