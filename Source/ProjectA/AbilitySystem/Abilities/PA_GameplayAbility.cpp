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
			// 어빌리티를 부여받자마자 활성화
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
			// 활성화했던 어빌리티 제거
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPA_PawnCombatComponent* UPA_GameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	// 폰 전투 컴포넌트 인터페이스를 통해, 플레이어 or 적의 전투 컴포넌트를 반환
	if (UPA_PawnCombatComponent* PawnCombatComponent = Cast<IPawnCombatInterface>(GetAvatarActorFromActorInfo())->GetPawnCombatComponent())
	{
		return PawnCombatComponent;
	}

	return nullptr;
}
