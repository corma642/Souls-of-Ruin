// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PA_PawnCombatComponent.h"
#include "PA_EnemyCombatComponent.generated.h"

UCLASS()
class PROJECTA_API UPA_EnemyCombatComponent : public UPA_PawnCombatComponent
{
	GENERATED_BODY()
	
	///////////////////////////////////////////////////////////////////////////
	/* Interaction */
public:
	// 무기 피해 함수
	virtual void OnWeaponHitStartTargetActor(AActor* HitActor) override;

	// 무기 피해 종료 함수
	virtual void OnWeaponHitEndTargetActor(AActor* HitActor) override;
};
