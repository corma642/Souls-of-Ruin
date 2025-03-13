// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Shared/GA_SpawnWeapon.h"

#include "Items/Weapons/PA_BaseWeapon.h"

void UGA_SpawnWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	USkeletalMeshComponent* SkeletalMesh = ActorInfo->SkeletalMeshComponent.Get();

	// 무기 스폰 및 부착
	for (int i = 0; i < SpawnWeaponClasses.Num(); ++i)
	{
		SpawnWeapons.Add(GetWorld()->SpawnActor<APA_BaseWeapon>(SpawnWeaponClasses[i]));
		SpawnWeapons[i]->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNames[i]);
	}
}

void UGA_SpawnWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
