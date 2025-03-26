// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PawnCombatComponent.h"
#include "Items/Weapons/PA_BaseWeapon.h"

UPA_PawnCombatComponent::UPA_PawnCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPA_PawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, APA_BaseWeapon* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	// ���Ⱑ ��ȿ���� ������ �ߴ�
	check(InWeaponToRegister);

	// ������ ����� �±׸� �ʿ� �߰�
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// ���� �������� ���⸦ ����Ϸ��� ���
	if (bRegisterAsEquippedWeapon)
	{
		// ����/������ ������ ���� ��쿡�� �̸� ����, ������ ���ÿ� ������ ���⸦ ���
		CurrentEquippingWeaponTag = InWeaponTagToRegister;
	}
}

APA_BaseWeapon* UPA_PawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	// �±׸� ���� ĳ���Ϳ� ��ϵ� ���⸦ ��ȯ
	if (CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
	{
		return CharacterCarriedWeaponMap[InWeaponTagToGet];
	}

	return nullptr;
}

APA_BaseWeapon* UPA_PawnCombatComponent::GetCharacterCurrentEquippingWeapon() const
{
	// ���� ������ ������ �±װ� ��ȿ�� ���
	if (CurrentEquippingWeaponTag.IsValid())
	{
		// �±׸� ���� ĳ���Ϳ� ��ϵ� ���⸦ ������ ��ȯ
		GetCharacterCarriedWeaponByTag(CurrentEquippingWeaponTag);
	}

	return nullptr;
}
