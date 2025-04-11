// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "Interface/PA_PawnCombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

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
	// �� ���� ������Ʈ �������̽��� ����, �÷��̾� or ���� ���� ������Ʈ�� ��ȯ
	if (UPA_PawnCombatComponent* PawnCombatComponent = Cast<IPA_PawnCombatInterface>(GetAvatarActorFromActorInfo())->GetPawnCombatComponent())
	{
		return PawnCombatComponent;
	}

	return nullptr;
}

UPA_AbilitySystemComponent* UPA_GameplayAbility::GetPAAbilitySystemComponentFromActorInfo() const
{
	return Cast<UPA_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UPA_GameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(TargetASC || InSpecHandle.IsValid());

	// ��� ASC�� �����÷��� ����Ʈ ���� �ڵ� ����
	return GetPAAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UPA_GameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPA_SuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EPA_SuccessType::SuccessFul : EPA_SuccessType::Failed;

	return ActiveGameplayEffectHandle;
}
