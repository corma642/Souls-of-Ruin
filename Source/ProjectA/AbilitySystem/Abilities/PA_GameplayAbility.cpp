// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "Interface/PawnCombatInterface.h"

#include "PA_FunctionLibrary.h"

void UPA_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UPA_AbilitySystemComponent* ASC = GetPAAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();

	// ���� �� ��� ����Ǵ� ����Ʈ ����
	for (auto Effect : OngoingEffectsToJustApplyOnStart)
	{
		if (!Effect.Get()) continue;

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	// ȿ���� ������ ���ŵǴ� ����Ʈ ����
	if (IsInstantiated())
	{
		for (auto Effect : OngoingEffectsToRemoveOnEnd)
		{
			if (!Effect.Get()) continue;

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (ActiveGEHandle.WasSuccessfullyApplied())
				{
					RemoveOnEndEffectHandles.Add(ActiveGEHandle);
				}
			}
		}
	}
}

void UPA_GameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			// �����Ƽ�� �ο����ڸ��� Ȱ��ȭ
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UPA_GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// ȿ���� ������ ���ŵǴ� ����Ʈ ����
	if (IsInstantiated())
	{
		for (FActiveGameplayEffectHandle ActiveEffectHandle : RemoveOnEndEffectHandles)
		{
			if (ActiveEffectHandle.IsValid())
			{
				ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}
		RemoveOnEndEffectHandles.Empty();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			// Ȱ��ȭ�ߴ� �����Ƽ ����
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPA_PawnCombatComponent* UPA_GameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	//// �� ���� ������Ʈ �������̽��� ����, �÷��̾� or ���� ���� ������Ʈ�� ��ȯ
	if (UPA_PawnCombatComponent* PawnCombatComponent = Cast<IPawnCombatInterface>(GetAvatarActorFromActorInfo())->GetPawnCombatComponent())
	{
		return PawnCombatComponent;
	}

	return nullptr;
}

UPA_AbilitySystemComponent* UPA_GameplayAbility::GetPAAbilitySystemComponentFromActorInfo() const
{
	return Cast<UPA_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
