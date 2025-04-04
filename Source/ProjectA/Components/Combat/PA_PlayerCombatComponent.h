// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PA_PawnCombatComponent.h"
#include "PA_PlayerCombatComponent.generated.h"

UCLASS()
class PROJECTA_API UPA_PlayerCombatComponent : public UPA_PawnCombatComponent
{
	GENERATED_BODY()
	
public:
	// �±׸� ����, ���� �÷��̾ ��ϵ� ���⸦ �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	// ���� �÷��̾ ������ ���⸦ �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCurrentEquippingWeapon() const;

	// ���� �÷��̾ ������ ������ ���ط��� �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	float GetPlayerCurrentEquippingWeaponDamage() const;


	///////////////////////////////////////////////////////////////////////////
	/* Interaction */
public:
	// ���� ���� �Լ�
	virtual void OnWeaponHitStartTargetActor(AActor* HitActor) override;

	// ���� ���� ���� �Լ�
	virtual void OnWeaponHitEndTargetActor(AActor* HitActor) override;
};
