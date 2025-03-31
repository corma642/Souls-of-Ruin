// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DA_BaseStartUpData.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"

void UDA_BaseStartUpData::GiveToAbilitySystemComponent(UPA_AbilitySystemComponent* InASCToGive)
{
	check(InASCToGive);

	// ���� �� �ο� �� Ȱ��ȭ(OnGive) �����Ƽ �ο�
	GrantAbilities(ActivateOnGiveAbilities, InASCToGive);

	// ���� �� �ο�(OnTriggered) �����Ƽ �ο�
	GrantAbilities(ReactiveAbilities, InASCToGive);

	// ���� �� ���� �����÷��� ����Ʈ ����
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
