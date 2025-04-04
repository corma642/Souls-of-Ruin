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
	// 태그를 통해, 현재 플레이어에 등록된 무기를 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	// 현재 플레이어에 장착된 무기를 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCurrentEquippingWeapon() const;

	// 현재 플레이어에 장착된 무기의 피해량을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	float GetPlayerCurrentEquippingWeaponDamage() const;


	///////////////////////////////////////////////////////////////////////////
	/* Interaction */
public:
	// 무기 피해 함수
	virtual void OnWeaponHitStartTargetActor(AActor* HitActor) override;

	// 무기 피해 종료 함수
	virtual void OnWeaponHitEndTargetActor(AActor* HitActor) override;
};
