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

	// ���� ü��
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, CurrentHealth)

	// �ִ� ü��
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxHealth)

	// ���� ���¹̳�(���)
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, CurrentStamina)

	// �ִ� ���¹̳�(���)
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxStamina)

	// �ִ� �̵��ӵ�
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, MaxMovementSpeed)

	// ���ݷ�
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, AttackPower)

	// ����
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, Defense)

	// ���� ���ط�
	UPROPERTY(BlueprintReadOnly, Category = "Custom | Attribute")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UPA_AttributeSetBase, DamageTaken)

private:
	// �������� UI �������̽� ĳ��
	TWeakInterfacePtr<class IPA_PawnUIInterface> CachedPawnInterface;
};
