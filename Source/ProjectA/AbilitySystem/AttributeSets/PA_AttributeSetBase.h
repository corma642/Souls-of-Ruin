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

	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, CurrentHealth)

	// 최대 체력
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxHealth)

	// 현재 스태미나(기력)
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, CurrentStamina)

	// 최대 스태미나(기력)
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxStamina)

	// 최대 이동속도
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxMovementSpeed)

	// 공격력
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, AttackPower)

	// 방어력
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, Defense)

	// 받은 피해량
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, DamageTaken)

private:
	// 소유자의 UI 인터페이스 캐시
	TWeakInterfacePtr<class IPA_PawnUIInterface> CachedPawnInterface;
};
