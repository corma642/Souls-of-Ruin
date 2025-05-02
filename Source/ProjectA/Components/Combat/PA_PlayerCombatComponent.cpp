// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PlayerCombatComponent.h"
#include "Items/Weapons/PA_PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "PA_GameplayTags.h"

APA_PlayerWeapon* UPA_PlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	return Cast<APA_PlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTagToGet));
}

APA_PlayerWeapon* UPA_PlayerCombatComponent::GetPlayerCurrentEquippingWeapon() const
{
	return Cast<APA_PlayerWeapon>(GetCharacterCurrentEquippingWeapon());
}

float UPA_PlayerCombatComponent::GetPlayerCurrentEquippingWeaponDamage() const
{
	if (APA_PlayerWeapon* PlayerWeapon = GetPlayerCurrentEquippingWeapon())
	{
		return PlayerWeapon->PlayerWeaponData.BaseWeaponDamage;
	}
	return 0.0f;
}

void UPA_PlayerCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor)
{
	// �̹� ������ �迭�� �����ϴ� ���ʹ� ����
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// �����÷��� �̺�Ʈ �����͸� �����, �浹�� ����� ����
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;

	// �ڽſ��� �����÷��� �̺�Ʈ ����
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);

	// �ǰ� ���� ȿ��
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UPA_PlayerCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor)
{

}
