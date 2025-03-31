// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/PA_BaseWeapon.h"
#include "ProjectATypes/PA_StructTypes.h"
#include "PA_PlayerWeapon.generated.h"

UCLASS()
class PROJECTA_API APA_PlayerWeapon : public APA_BaseWeapon
{
	GENERATED_BODY()
	
public:
	// 플레이어 무기 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | WeaponData")
	FPlayerWeaponData PlayerWeaponData;
};
