// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PA_PawnCombatComponent.h"
#include "PA_PlayerCombatComponent.generated.h"

class APA_CharacterPlayer;

UCLASS()
class PROJECTA_API UPA_PlayerCombatComponent : public UPA_PawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UPA_PlayerCombatComponent();

	// 태그를 통해, 현재 플레이어에 등록된 무기를 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	// 현재 플레이어에 장착된 무기를 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCurrentEquippingWeapon() const;

	// 현재 플레이어에 장착된 무기의 피해량을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	float GetPlayerCurrentEquippingWeaponDamage() const;

	// 현재 플레이어에 장착된 무기의 스킬 대미지를 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	float GetPlayerCurrentEquippingWeaponSkillDamage(const int InSkillNumber) const;


	///////////////////////////////////////////////////////////////////////////
	/* Interaction */
public:
	// 무기 피해 함수
	virtual void OnWeaponHitStartTargetActor(AActor* HitActor, const FHitResult& HitResult) override;

	// 무기 피해 종료 함수
	virtual void OnWeaponHitEndTargetActor(AActor* HitActor, const FHitResult& HitResult) override;

private:
	// 플레이어 클래스
	APA_CharacterPlayer* Player = nullptr;
};
