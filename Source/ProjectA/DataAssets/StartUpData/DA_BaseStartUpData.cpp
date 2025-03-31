// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DA_BaseStartUpData.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"

void UDA_BaseStartUpData::GiveToAbilitySystemComponent(UPA_AbilitySystemComponent* InASCToGive)
{
	check(InASCToGive);

	// 시작 시 부여 및 활성화(OnGive) 어빌리티 부여
	GrantAbilities(ActivateOnGiveAbilities, InASCToGive);

	// 시작 시 부여(OnTriggered) 어빌리티 부여
	GrantAbilities(ReactiveAbilities, InASCToGive);

	// 시작 시 적용 게임플레이 이펙트 적용
	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<class UGameplayEffect>& GameplayEffect : StartUpGameplayEffects)
		{
			if (GameplayEffect)
			{
				UGameplayEffect* EffectCDO = GameplayEffect->GetDefaultObject<UGameplayEffect>();
				InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, 1, InASCToGive->MakeEffectContext());
			}
		}
	}
}

void UDA_BaseStartUpData::GrantAbilities(const TArray<TSubclassOf<class UPA_GameplayAbility>>& InAbilities, UPA_AbilitySystemComponent* InASCToGive)
{
	for (const TSubclassOf<UPA_GameplayAbility>& Ability : InAbilities)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability);
			AbilitySpec.SourceObject = this;
			AbilitySpec.Level = 1;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
