// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PawnCombatComponent.h"
#include "Items/Weapons/PA_BaseWeapon.h"
#include "Components/BoxComponent.h"

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


	// ������ ���ۿ� ���� / ���� �̺�Ʈ ��������Ʈ��
	// ���� ���ظ� ������ �Լ�, �����ϴ� �Լ��� ���ε�
	InWeaponToRegister->OnWeaponTargetHitStart.BindUObject(this, &UPA_PawnCombatComponent::OnWeaponHitStartTargetActor);
	InWeaponToRegister->OnWeaponTargetHitEnd.BindUObject(this, &UPA_PawnCombatComponent::OnWeaponHitEndTargetActor);
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
		return GetCharacterCarriedWeaponByTag(CurrentEquippingWeaponTag);
	}

	return nullptr;
}

void UPA_PawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	APA_BaseWeapon* Weapon = GetCharacterCurrentEquippingWeapon();
	check(Weapon);

	// ���� ������ �ݸ��� ��ȯ
	if (ToggleDamageType == EToggleDamageType::LeftWeapon)
	{
		if (bShouldEnable)
		{
			Weapon->GetLeftWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Weapon->GetLeftWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}
	}
	// ������ ������ �ݸ��� ��ȯ
	else if (ToggleDamageType == EToggleDamageType::RightWeapon)
	{
		if (bShouldEnable)
		{
			Weapon->GetRightWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Weapon->GetRightWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}
	}
}

void UPA_PawnCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor)
{
}

void UPA_PawnCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor)
{
}
