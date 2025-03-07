// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "PA_AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECTA_API UPA_AttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPA_AttributeSetBase();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute | Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute | Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxHealth)
};
