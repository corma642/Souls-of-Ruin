// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_PawnCombatComponent.h"
#include "Items/Weapons/PA_BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Characters/PA_CharacterEnemy.h"

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


	// 무기의 상호작용 시작 / 종료 이벤트 델리게이트와
	// 무기 피해를 입히는 함수, 종료하는 함수와 바인딩
	InWeaponToRegister->OnWeaponTargetHitStart.BindUObject(this, &UPA_PawnCombatComponent::OnWeaponHitStartTargetActor);
	InWeaponToRegister->OnWeaponTargetHitEnd.BindUObject(this, &UPA_PawnCombatComponent::OnWeaponHitEndTargetActor);
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
		return GetCharacterCarriedWeaponByTag(CurrentEquippingWeaponTag);
	}

	return nullptr;
}

void UPA_PawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	// 무기로 피해를 입히는 경우
	APA_BaseWeapon* Weapon = GetCharacterCurrentEquippingWeapon();
	if (ToggleDamageType == EToggleDamageType::LeftWeapon ||
		ToggleDamageType == EToggleDamageType::RightWeapon)
	{
		check(Weapon);
	}

	// 콜리전 활성화
	if (bShouldEnable)
	{
		switch (ToggleDamageType)
		{
		case EToggleDamageType::LeftWeapon:		// 왼쪽 무기
			Weapon->GetLeftWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly); break;

		case EToggleDamageType::RightWeapon:	// 오른쪽 무기
			Weapon->GetRightWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly); break;

		case EToggleDamageType::LeftHand:		// 왼손
			Cast<APA_CharacterEnemy>(GetOwner())->GetLeftHandCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly); break;

		case EToggleDamageType::RightHand:		// 오른손
			Cast<APA_CharacterEnemy>(GetOwner())->GetRightHandCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly); break;
		}
	}
	// 콜리전 비활성화
	else
	{
		switch (ToggleDamageType)
		{
		case EToggleDamageType::LeftWeapon:		// 왼쪽 무기
			Weapon->GetLeftWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision); break;

		case EToggleDamageType::RightWeapon:	// 오른쪽 무기
			Weapon->GetRightWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision); break;

		case EToggleDamageType::LeftHand:		// 왼손
			Cast<APA_CharacterEnemy>(GetOwner())->GetLeftHandCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision); break;

		case EToggleDamageType::RightHand:		// 오른손
			Cast<APA_CharacterEnemy>(GetOwner())->GetRightHandCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision); break;
		}

		// 오버랩 배열 초기화
		OverlappedActors.Empty();
	}
}

void UPA_PawnCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
}

void UPA_PawnCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
}
