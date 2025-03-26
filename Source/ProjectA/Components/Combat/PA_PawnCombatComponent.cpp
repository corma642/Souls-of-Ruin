// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PawnCombatComponent.h"
#include "Items/Weapons/PA_BaseWeapon.h"

UPA_PawnCombatComponent::UPA_PawnCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPA_PawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, APA_BaseWeapon* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	// 무기가 유효하지 않으면 중단
	check(InWeaponToRegister);

	// 스폰된 무기와 태그를 맵에 추가
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// 현재 장착중인 무기를 등록하려는 경우
	if (bRegisterAsEquippedWeapon)
	{
		// 무장/비무장의 개념이 없는 경우에는 이를 통해, 스폰과 동시에 장착한 무기를 등록
		CurrentEquippingWeaponTag = InWeaponTagToRegister;
	}
}

APA_BaseWeapon* UPA_PawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	// 태그를 통해 캐릭터에 등록된 무기를 반환
	if (CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
	{
		return CharacterCarriedWeaponMap[InWeaponTagToGet];
	}

	return nullptr;
}

APA_BaseWeapon* UPA_PawnCombatComponent::GetCharacterCurrentEquippingWeapon() const
{
	// 현재 장착한 무기의 태그가 유효한 경우
	if (CurrentEquippingWeaponTag.IsValid())
	{
		// 태그를 통해 캐릭터에 등록된 무기를 가져와 반환
		GetCharacterCarriedWeaponByTag(CurrentEquippingWeaponTag);
	}

	return nullptr;
}
