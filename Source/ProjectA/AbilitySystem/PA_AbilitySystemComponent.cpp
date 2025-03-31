// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "Abilities/PA_PlayerGameplayAbility.h"

#include "PA_GameplayTags.h"

void UPA_AbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	// ���� Ȱ��ȭ ������ �����Ƽ ��ȸ
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// �ش� �����Ƽ�� �Է� �±װ� �Է��� �Է� �±��� ���
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			// �����Ƽ �Է� �±װ� ��� �Է��̰�, �����Ƽ�� ���� Ȱ��ȭ ���� ���
			if (InInputTag.MatchesTag(PA_GameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
			{
				// Ȱ��ȭ �Ǿ� �ִ� �����Ƽ�� ��Ȱ��ȭ �Ѵ�.
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			else
			{
				// �����Ƽ�� Ȱ��ȭ �Ѵ�.
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPA_AbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	// �Է� �±װ� ��ȿ���� �ʰų�, �Է� �±׿� MustBeHeld�� ������� ������ ��ȯ
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(PA_GameplayTags::InputTag_MustBeHeld)) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// �����Ƽ �Է� �±׿� MustBeHeld�� ����ְ�, ���� �����Ƽ�� Ȱ��ȭ ���� ���
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			// Ȱ��ȭ �Ǿ� �ִ� �����Ƽ�� ��Ȱ��ȭ �Ѵ�.
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

		// ���� �����Ƽ �ο� �� ���� �ڵ� ����
		GrantedPlayerWeaponAbilities.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UPA_AbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities()
{
	if (!GrantedPlayerWeaponAbilities.IsEmpty())
	{
		// �÷��̾ �ο��� ���� �����Ƽ ����
		for (const FGameplayAbilitySpecHandle& AbilityHandle : GrantedPlayerWeaponAbilities)
		{
			ClearAbility(AbilityHandle);
		}
	}

	GrantedPlayerWeaponAbilities.Empty();
}
