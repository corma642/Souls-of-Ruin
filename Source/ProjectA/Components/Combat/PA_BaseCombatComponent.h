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
	// ���� ĳ���Ϳ� ������ ����
	UPROPERTY()
	TMap<FName, TObjectPtr<class APA_BaseWeapon>> CharacterAttachedWeapon;
};
