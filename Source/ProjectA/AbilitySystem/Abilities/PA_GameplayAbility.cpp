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

	// 시작 후 계속 적용되는 이펙트 적용
	for (auto Effect : OngoingEffectsToJustApplyOnStart)
	{
		if (!Effect.Get()) continue;

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	// 효과가 끝나면 제거되는 이펙트 적용
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
			// 어빌리티를 부여받자마자 활성화
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UPA_GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 효과가 끝나면 제거되는 이펙트 제거
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
			// 활성화했던 어빌리티 제거
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPA_PawnCombatComponent* UPA_GameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	// 폰 전투 컴포넌트 인터페이스를 통해, 플레이어 or 적의 전투 컴포넌트를 반환
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

	// 대상 ASC에 게임플레이 이펙트 스펙 핸들 적용
	return GetPAAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UPA_GameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPA_SuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EPA_SuccessType::SuccessFul : EPA_SuccessType::Failed;

	return ActiveGameplayEffectHandle;
}
