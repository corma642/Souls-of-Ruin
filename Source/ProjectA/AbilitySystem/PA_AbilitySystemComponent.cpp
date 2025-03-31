// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "Abilities/PA_PlayerGameplayAbility.h"

#include "PA_GameplayTags.h"

void UPA_AbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	// 현재 활성화 가능한 어빌리티 순회
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 해당 어빌리티의 입력 태그가 입력한 입력 태그인 경우
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			// 어빌리티 입력 태그가 토글 입력이고, 어빌리티가 현재 활성화 중인 경우
			if (InInputTag.MatchesTag(PA_GameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
			{
				// 활성화 되어 있는 어빌리티를 비활성화 한다.
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			else
			{
				// 어빌리티를 활성화 한다.
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPA_AbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	// 입력 태그가 유효하지 않거나, 입력 태그에 MustBeHeld가 들어있지 않으면 반환
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(PA_GameplayTags::InputTag_MustBeHeld)) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 어빌리티 입력 태그에 MustBeHeld가 들어있고, 현재 어빌리티가 활성화 중인 경우
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			// 활성화 되어 있는 어빌리티를 비활성화 한다.
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UPA_AbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FPlayerWeaponAbilitySet>& InAbilitiesToGrant)
{
	if (InAbilitiesToGrant.IsEmpty()) return;

	for (const FPlayerWeaponAbilitySet& AbilitySet : InAbilitiesToGrant)
	{
		if (!AbilitySet.IsValid()) return;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant.Get());
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		// 무기 어빌리티 부여 및 스펙 핸들 저장
		GrantedPlayerWeaponAbilities.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UPA_AbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities()
{
	if (!GrantedPlayerWeaponAbilities.IsEmpty())
	{
		// 플레이어에 부여한 무기 어빌리티 제거
		for (const FGameplayAbilitySpecHandle& AbilityHandle : GrantedPlayerWeaponAbilities)
		{
			ClearAbility(AbilityHandle);
		}
	}

	GrantedPlayerWeaponAbilities.Empty();
}
