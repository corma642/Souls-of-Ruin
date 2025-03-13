// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "GA_SpawnWeapon.generated.h"

UCLASS()
class PROJECTA_API UGA_SpawnWeapon : public UPA_GameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 스폰 무기 클래스 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom")
	TArray<TSubclassOf<class APA_BaseWeapon>> SpawnWeaponClasses;

	// 스폰한 무기 배열
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Custom")
	TArray<class APA_BaseWeapon*> SpawnWeapons;

	// 부착할 소켓 이름 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom")
	TArray<FName> SocketNames;
};
