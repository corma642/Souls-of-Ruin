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
	UFUNCTION(BlueprintPure, Category = "Custom | Combat")
	class APA_PlayerWeapon* GetPlayerCarriedByTag(FGameplayTag InWeaponTagToGet) const;
};
