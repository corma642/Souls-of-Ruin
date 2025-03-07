// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"
#include "GameplayEffectExtension.h"

UPA_AttributeSetBase::UPA_AttributeSetBase()
{
}

void UPA_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


}
