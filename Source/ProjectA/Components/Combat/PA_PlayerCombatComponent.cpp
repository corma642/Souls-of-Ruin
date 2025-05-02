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
	// 이미 오버랩 배열에 존재하는 액터는 무시
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// 게임플레이 이벤트 데이터를 만들어, 충돌한 대상을 저장
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;

	// 자신에게 게임플레이 이벤트 전달
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);

	// 피격 정지 효과
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UPA_PlayerCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor)
{

}
