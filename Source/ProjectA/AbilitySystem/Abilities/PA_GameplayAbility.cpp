// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Characters/PA_CharacterEnemy.h"

void UPA_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
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
}

APA_CharacterPlayer* UPA_GameplayAbility::GetPlayerCharacterFromActorInfo() const
{
	return Cast<APA_CharacterPlayer>(GetAvatarActorFromActorInfo());
}

APA_CharacterEnemy* UPA_GameplayAbility::GetEnemyCharacterFromActorInfo() const
{
	return Cast<APA_CharacterEnemy>(GetAvatarActorFromActorInfo());
}
