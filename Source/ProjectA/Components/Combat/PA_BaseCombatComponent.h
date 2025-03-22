// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PA_BaseCombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTA_API UPA_BaseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPA_BaseCombatComponent();

	TMap<FName, TObjectPtr<class APA_BaseWeapon>> const GetCharacterAttachedWeapon() const { return CharacterAttachedWeapon; }

	//void AddCharacterAttachedWeapon

private:
	// 현재 캐릭터에 부착된 무기
	UPROPERTY()
	TMap<FName, TObjectPtr<class APA_BaseWeapon>> CharacterAttachedWeapon;
};
