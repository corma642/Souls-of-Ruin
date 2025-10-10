// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PlayerCombatComponent.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Items/Weapons/PA_PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

UPA_PlayerCombatComponent::UPA_PlayerCombatComponent()
{
	Player = Cast<APA_CharacterPlayer>(GetOwner());
}

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
		return PlayerWeapon->PlayerWeaponData.WeaponStats.GetValueAtLevel(1);
	}
	return 0.0f;
}

float UPA_PlayerCombatComponent::GetPlayerCurrentEquippingWeaponSkillDamage(const int InSkillNumber) const
{
	// 현재 플레이어에 장착된 무기의 스킬 중, 특정 스킬의 대미지 반환
	if (APA_PlayerWeapon* PlayerWeapon = GetPlayerCurrentEquippingWeapon())
	{
		if (PlayerWeapon->PlayerWeaponData.WeaponSkillAbilities.IsValidIndex(InSkillNumber - 1))
		{
			if (PlayerWeapon->PlayerWeaponData.WeaponSkillAbilities[InSkillNumber - 1].IsValid())
			{
				// 무기 스킬 인덱스에 접근해서 해당 무기 스킬의 대미지 반환
				return PlayerWeapon->PlayerWeaponData.WeaponSkillAbilities[InSkillNumber - 1].SkillDamage.GetValue();
			}
		}
	}
	return 0.0f;
}

void UPA_PlayerCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
	// 이미 오버랩 배열에 존재하는 액터는 무시
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();

	// 무적 여부
	bool bIsInvincible = UPA_FunctionLibrary::NativeIsInvincible(HitActor);
	if (bIsInvincible)
	{
		return;
	}

	// 게임플레이 이벤트 데이터를 만들어, 충돌한 대상을 저장
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;
	Payload.EventMagnitude = 1.0f;

	// 피격 HitResult 보내기
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddHitResult(HitResult, false);
	Payload.ContextHandle = EffectContext;

	// 자신에게 근접 공격 이벤트 전달
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);

	// 자신에게 피격 정지 효과 이벤트 전달
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Player_Event_HitPause, Payload);
}

void UPA_PlayerCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor, const FHitResult& HitResult)
{

}
