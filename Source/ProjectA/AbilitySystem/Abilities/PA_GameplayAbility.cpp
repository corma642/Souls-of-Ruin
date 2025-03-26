// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Interface/PawnCombatInterface.h"

#include "PA_FunctionLibrary.h"

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
	if (UPA_PawnCombatComponent* PawnCombatComponent = Cast<IPawnCombatInterface>(GetAvatarActorFromActorInfo())->GetPawnCombatComponent())
	{
		return PawnCombatComponent;
	}

	return nullptr;
}
