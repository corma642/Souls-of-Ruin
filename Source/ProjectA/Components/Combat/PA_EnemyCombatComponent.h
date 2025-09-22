// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PA_PawnCombatComponent.h"
#include "PA_EnemyCombatComponent.generated.h"

class APA_CharacterEnemy;

UCLASS()
class PROJECTA_API UPA_EnemyCombatComponent : public UPA_PawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UPA_EnemyCombatComponent();


	///////////////////////////////////////////////////////////////////////////
	/* Interaction */
public:
	// 무기 피해 함수
	virtual void OnWeaponHitStartTargetActor(AActor* HitActor, const FHitResult& HitResult) override;

	// 무기 피해 종료 함수
	virtual void OnWeaponHitEndTargetActor(AActor* HitActor, const FHitResult& HitResult) override;

private:
	// 적 클래스
	APA_CharacterEnemy* Enemy = nullptr;
};
